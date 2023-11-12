#pragma once
// C++
#include <string>
#include <string_view>

// исходный размер массива для приема текста
#define BUFLEN 1024

// метод приема текста неизвестной длины
// выбрать один из двух
// если не выбрать ни одного, то будет прием в массив фиксированного размера
#define READING_TAIL
//#define DOUBLING_LOOP

class Worker
{
private:
    std::string_view variant;
    int work_sock;
    std::string str_read();
    void auth();
    void calculate();
    template <typename T> void calc();
public:
    Worker()=delete;
    Worker(std::string_view v);
    void operator()(int sock);
};
