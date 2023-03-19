#ifndef create_socket_hpp
#define create_socket_hpp
#include<iostream>
#include <sys/socket.h>
#include<netinet/in.h>
class create_socket
{
private:
    int sockfd;
    struct sockaddr_in address;
public:
// Create a socket to a local address and port
    create_socket(int domain, int sevice, int protocol, int port, u_long interface)
    {
        address.sin_family = domain;
        address.sin_port = htons(port);//convert the port number from `host` byte order `to network` byte order.
        address.sin_addr.s_addr = htonl(interface);// convert the byte order of a 32-bit unsigned integer from host byte order to network byte order. 
       if ((sockfd = socket(domain, sevice, protocol)) == 0)
       {
            std::cerr<<"Error creating socket"<<std::endl;
            exit(EXIT_FAILURE);
       }
    }
    ~create_socket();
};

#endif