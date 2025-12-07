# Networks

Network applications are typically based on the **server-client model**. That is, a client requests resources from a server, which both handles and processes those resources.

There are different types of networks that differ in their size and reach. **System Area Networks (SANs)** may span a small room via switched Ethernet. **Local Area Networks (LAN)** are small networks of hosts that may span a building or campus, often connected by Ethernet. **Wide Area Networks (WAN)** may span whole countries, such as phone lines.

An **internet** (lowercase) is when multiple networks connect to each other. The largest such network is the **Internet** (capital I), the Global IP Internet we use to access google and other websites.

Internets connect multiple isolated LANs via **routers**, which themselves are connected by a WAN. Packets are sent across routers, which forward them to their end destination based on routing tables. Incompatible LANs are able to communicate with each other by adhering to an **Internet Protocol (IP)**, a set of rules that defines addressing and routing for how networks must message each other. The unit of data transferred is called a **packet**, which contains **headers** and a **payload**.

The Global IP Network is based on TCP/IP (Transmission Control Protocol / Internet Protocol). IP handles addressing and routing, while TCP ensures reliable, ordered delivery of data. At the programming interface level, TCP behaves similarly to UNIX I/O: data is read and written using sockets. In this sense, the Internet is an I/O peripheral to the operating system whereby the exact mechanism that fetches and sends data to its destination is performed by a complicated network of routers.

In this protocol, host machines are given an **IP address**, which is mapped to an easier to remember **domain name**.

    ip_address  domain_name
    127.0.0.1   http://localhost

Processes on an internet communicate over a **connection**. The endpoint of a connection is a **socket**. A **port** defines a process running on the host and connected to that is bound to the socket.

The Internet (capital I) maintains a distributed database that maps human-readable domain names to IP addresses, following the **Domain Name System (DNS)**. Internet service providers maintain these mappings, ensuring that when you go to google.com, it is able to find the correct IP address to establish a connection with. Multiple IP addresses can map to one domain name (for load balancing or redundancy).

On Linux, sockets are modeled as file descriptors and must be opened and closed just like regular files. The RIO library in C is a common way to interface with incoming packets transferred over the network, providing a way to read/write buffers of bytes to and from the server or client.

    #include "csapp.h"

    int main(int argc, char **argv) {
        int clientfd;
        char *host, *port, buf[MAXLINE];
        rio_t rio;

        host = argv[1];
        port = argv[2];

        clientfd = Open_clientfd(host, port);
        Rio_readinitb(&rio, clientfd);

        while (Fgets(buf, MAXLINE, stdin) != NULL) {
            Rio_writen(clientfd, buf, strlen(buf));
            Rio_readlineb(&rio, buf, MAXLINE);
            Fputs(buf, stdout);
        }

        Close(clientfd);
        exit(0);
    }

The **World Wide Web** is a service that runs on top of the Internet, that uses **HyperText Transfer Protocol (HTTP/HTTPS)** to communicate. Once a TCP connection is established with client and server, they use HTTP requests/responses to send and receive web pages.

HTTP packets contain request, header, and data lines. A simple request would be

    GET /index.html HTTP/1.1
    Host: www.example.com

The server responds with an HTTP status line, headers, and an optional message body (e.g., HTML content).