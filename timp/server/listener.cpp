// C++
#include <iostream>
#include <system_error>

// POSIX
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// self
#include "listener.h"
#include "server_error.h"

Listener::Listener(unsigned short port, int qlen):
    sock(socket(AF_INET, SOCK_STREAM, 0)),				// create listening socket
    self_addr(new sockaddr_in),							// allocate memory fo self address
    foreign_addr(new sockaddr_in),						// allocate memory fo foreign address
    queueLen(qlen)
{
    // check error for socket creation
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "Socket error");
    int on = 1;
    // set soket for fast reusing when server restarting
    int rc = setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category(), "Socket error");
    // fill self address
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port);
    self_addr->sin_addr.s_addr = 0;
    // bind listening socket for self address
    if (bind(sock,
             reinterpret_cast<const sockaddr*>(self_addr.get()),
             sizeof (sockaddr_in)) == -1)
        throw std::system_error(errno, std::generic_category(), "Bind error");
}

Listener::~Listener()
{
    close(sock);
}

void Listener::Run(Worker & worker)
{
    // listen for connection
    if (listen(sock, queueLen) == -1)
        throw std::system_error(errno, std::generic_category(), "Listen error");
    socklen_t socklen = sizeof (sockaddr_in);
    while(true) {
        int work_sock = accept(sock,
                               reinterpret_cast<sockaddr*>(foreign_addr.get()),
                               &socklen);
        if (work_sock == -1)
            throw std::system_error(errno, std::generic_category(), "Accept error");
        std::string ip_addr(inet_ntoa(foreign_addr->sin_addr));
        std::clog << "log: Connection established with " << ip_addr <<std::endl;
        try {
            worker(work_sock);	// serve the client
        } catch (std::system_error &e) {
            std::cerr << e.what() << "\nConnection with " << ip_addr << " aborted\n";
        } catch (auth_error &e) {
            std::cerr << e.what() << "\nConnection with " << ip_addr << " aborted\n";
            send(work_sock, "ERR", 3, 0);
        } catch (vector_error &e) {
            std::cerr << e.what() << "\nConnection with " << ip_addr << " aborted\n";
        } catch (std::bad_alloc &e) {
        	std::cerr << e.what() << ": verctor size too big\nConnection with " << ip_addr << " aborted\n";
        }
        close(work_sock);
        std::clog << "log: Connection close\n";
    }
}
