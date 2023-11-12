// C++
#include <iostream>
#include <memory>
#include <random>

// POSIX
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

//Crypto++
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

// self
#include "worker.h"
#include "server_error.h"

static constexpr std::string_view login = "user";
static constexpr std::string_view password = "P@ssW0rd";

Worker::Worker(std::string_view v):variant(v), work_sock(-1)
{
    std::clog << "log: Select variant " << variant << std::endl;
}

void Worker::operator()(int sock)
{
    work_sock = sock;
    auth();
    calculate();
}

template <typename T> void Worker::calc()
{
    uint32_t num_vectors, vector_len;
    int rc;
    std::clog << "log: Start calculate with type " << typeid(T).name() << std::endl;
    rc = recv(work_sock, &num_vectors, sizeof num_vectors, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Recv number of vetors error");
    std::clog << "log: Numbers of vectors " << num_vectors << std::endl;
    for (uint32_t i = 0; i < num_vectors; ++i) {
        rc = recv(work_sock, &vector_len, sizeof vector_len, 0);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Recv vector size error");
        std::clog << "log: Vector " << i <<" size " << vector_len << std::endl;
        std::unique_ptr<T[]> data (new T[vector_len]);
        std::clog << "log: Memory allocated at "  << data.get() << std::endl;
        rc = recv(work_sock, data.get(), sizeof(T) * vector_len, 0);
        std::clog << "log: Received "  << rc << " bytes of vector\n";
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Recv vector error");
        else if (sizeof(T) * vector_len != (uint32_t)rc) {
            throw vector_error("Vector error: mismatch actual and expected size");
        }
        T sum = 0;

        for (uint32_t i = 0; i < vector_len; ++i) {
            sum += data[i];
            if (std::is_integral<T>::value) {               // if interer types
                if (std::is_signed<T>::value) {             // if signed
                    if (data[i] > 0 and sum < 0 and (sum - data[i]) > 0) { //if overflow
                        sum = std::numeric_limits<T>::max();
                        break;
                    } else if (data[i] < 0 and sum > 0 and (sum - data[i]) < 0) {  //if undeflow
                        sum = std::numeric_limits<T>::min();
                        break;
                    }
                } else if (sum < data[i]) {                 // if unsigned, if overflow
                    sum = std::numeric_limits<T>::max();
                    break;
                }
            }
        }
        rc = send(work_sock, &sum, sizeof(T), 0);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Send result error");
        std::clog << "log: Sended vector summ " << sum << std::endl;
    }
}

void Worker::calculate()
{
    if (variant == "float")
        Worker::calc<float>();
    else if (variant == "double")
        calc<double>();
    else if (variant == "uint16_t")
        calc<uint16_t>();
    else if (variant == "int16_t")
        calc<int16_t>();
    else if (variant == "uint32_t")
        calc<uint32_t>();
    else if (variant == "int32_t")
        calc<int32_t>();
    else if (variant == "uint64_t")
        calc<uint64_t>();
    else
        calc<int64_t>();
}

void Worker::auth()
{
    namespace cpp = CryptoPP;
    int rc;
    std::mt19937_64 gen(time(nullptr));
    if (str_read() != login)
        throw auth_error("Auth error: unknown user");
    std::clog << "log: username ok\n";
    uint64_t rnd = gen();
    std::string salt, message;
    cpp::StringSource((const cpp::byte*)&rnd,
                      8,
                      true,
                      new cpp::HexEncoder(new cpp::StringSink(salt)));
    rc = send(work_sock, salt.c_str(), 16, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Send salt error");
    std::clog << "log: sending SALT " << salt << std::endl;
    cpp::Weak::MD5 hash;
    cpp::StringSource(salt + std::string(password),
                      true,
                      new cpp::HashFilter(hash, new cpp::HexEncoder(new cpp::StringSink(message))));
    std::clog << "log: waiting MESSAGE " << message << std::endl;
    if (str_read() != message)
        throw auth_error("Auth error: password mismatch");
    std::clog <<"log: auth success, sending OK\n";
    rc = send(work_sock, "OK", 2, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Send OK error");
}

#if defined DOUBLING_LOOP
// метод удвоения в цикле
std::string Worker::str_read()
{
    int rc;
    int buflen = BUFLEN;
    std::unique_ptr <char[]> buf(new char[buflen]);
    while (true) {
        rc = recv(work_sock, buf.get(), buflen, MSG_PEEK);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Recv string error");
        if (rc < buflen)
            break;
        buflen *= 2;
        buf = std::unique_ptr<char[]>(new char[buflen]);
    }
    std::string res(buf.get(), rc);
    rc = recv(work_sock, nullptr, rc, MSG_TRUNC);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Clear bufer error");
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}

#elif defined READING_TAIL

// метод дочитывание хвоста
std::string Worker::str_read()
{
    int rc;
    int buflen = BUFLEN;
    std::unique_ptr <char[]> buf(new char[buflen]);
    rc = recv(work_sock, buf.get(), buflen, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Recv string error");
    std::string res(buf.get(), rc);
    if (rc == buflen) {
        int tail_size;
        rc = ioctl(work_sock, FIONREAD, &tail_size);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "IOCTL error");
        if (tail_size > 0) {
            if (tail_size > buflen)
                buf = std::unique_ptr<char[]>(new char[tail_size]);
            rc = recv(work_sock, buf.get(), tail_size, 0);
            if (rc == -1)
                throw std::system_error(errno, std::generic_category(), "Recv string error");
            res.append(buf.get(), rc);
        }
    }
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}

#else
// метод достаточного буфера (ненадежный)
std::string Worker::str_read()
{
    int buflen = BUFLEN;
    std::unique_ptr <char[]> buf(new char[buflen]);
    int rc = recv(work_sock, buf.get(), buflen, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Recv string error");
    std::string res(buf.get(), rc);
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}
#endif
