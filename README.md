# webserv
# Socket
A socket is a software object that enables network communication between two processes or applications running on different machines or within the same machine. It is a fundamental building block of network programming, allowing processes to send and receive data over a network.

A socket is identified by a unique combination of an IP address and a port number. When a process wants to communicate with another process, it creates a socket object and binds it to a specific IP address and port number. The other process can then use the same IP address and port number to connect to the socket and communicate with the first process.

There are two types of sockets: TCP sockets and UDP sockets. TCP sockets provide reliable, stream-oriented communication between processes, while UDP sockets provide connectionless, datagram-based communication.

Socket programming involves using a variety of socket functions to create, configure, connect, send, and receive data over sockets. These functions are typically provided by the operating system and can be accessed using socket APIs in programming languages such as C, C++, Python, Java, and others.

Sockets are widely used in client-server applications, web applications, email applications, and many other networked applications. They provide a flexible and powerful means of exchanging data over a network and have become a key part of modern network programming. 
# To create socket
you have to use the socket function to create a socket object.
```#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        // error handling
    }

    // use the socket object
    // ...

    return 0;
}
```
In this example, the `socket` function creates a socket object of the specified type (`SOCK_STREAM` for a TCP socket) and returns a file descriptor (`sockfd`) that can be used to reference the socket object in subsequent function calls.

Note that there are other options you can pass to the `socket` function depending on your needs, such as the protocol to use (0 for the default protocol) and the domain (e.g., `AF_INET`for IPv4 or `AF_INET6` for IPv6). You'll also need to set up the socket address and bind the socket to a port if you want to accept incoming connections.
The `AF_UNIX` domain is used for communication between processes on the same machine using Unix domain sockets.
By specifying the domain parameter in the `socket` function, you are telling the operating system which protocol family to use when creating the socket.

`domain`: Specifies the protocol family or domain that the socket will use. For example, `AF_INET` for `IPv4`, AF_INET6 for IPv6, or AF_UNIX for Unix domain sockets.
`type`: Specifies the type of socket to create. This can be SOCK_STREAM for a TCP socket or `SOCK_DGRAM` for a UDP socket, among others.
`protocol`: Specifies the specific protocol to be used with the socket. For example, `IPPROTO_TCP` for TCP or `IPPROTO_UDP` for UDP. If the protocol argument is set to 0, the socket function will choose the default protocol for the specified domain and type.
In general, when you call the `socket` function, you need to specify the domain, type, and protocol parameters that are appropriate for your specific networking needs. The exact values to use for each parameter will depend on the specific application you are developing and the network protocols you need to work with.

After creating the socket, you can use other socket functions (e.g., `bind`, `connect`, `listen`, `accept`) to configure the socket and establish connections. You can also use functions such as `send` and `recv` to send and receive data over the socket, and `close` to release the socket resources when you are done with it.

`htons` function is used to convert the port number from host byte order to network byte order.
# 