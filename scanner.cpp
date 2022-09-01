#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

#define MIN_PORT 0 // can probably delete
#define MAX_PORT 65353

void scan_ports(struct in_addr* ip_addr, unsigned start_port, unsigned end_port);

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: ./scanner ip_addr start_port end_port\n";
    }

    struct in_addr ip_addr;

    // TODO: add support for ipv6 ip addresses
    // note: no need for ip addr verification -- inet_pton does this
    if (inet_pton(AF_INET, argv[1], &ip_addr) != 1) {
        std::cerr << "Error: Invalid IP address\n";
        exit(EXIT_FAILURE);
    }

    std::cout << ((char*) &ip_addr); 

    // char* start_port = argv[2];
    // char* end_port = argv[3];
    
    unsigned start_port = std::stoi(argv[2]);
    unsigned end_port = std::stoi(argv[3]); 

    if (start_port > MAX_PORT || end_port > MAX_PORT || start_port > end_port) {
        std::cerr << "Error: Invalid port range\n";
        exit(EXIT_FAILURE); 
    }
    std::cout << start_port << " " << end_port;


    scan_ports(&ip_addr, start_port, end_port);
}

void scan_ports(struct in_addr* ip_addr, unsigned start_port, unsigned end_port) {
    struct sockaddr_in addr; 
    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_addr = *ip_addr; 
    addr.sin_family = AF_INET; 


    for(int port = start_port; port <= end_port; ++port) {
        //std::cout << "PORT " << port;
        
        // getaddrinfo(ip_addr, )
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            std::cerr << "Error: Failed to create socket\n";
            exit(EXIT_FAILURE);
        }
        if (fcntl(fd, F_SETFL, O_NONBLOCK)) {
            std::cerr << "Error: Failed to create socket\n";
            exit(EXIT_FAILURE);      
        }

        addr.sin_port = htons(port); 


        auto res = connect(fd, (const struct sockaddr*) &addr, sizeof(addr));
      //  std::cout << res << "\n";
        if (res == 0) {
            std::cout << "Port " << port << " :OPEN"; 
        }
        else if (res == -1 && errno != EINPROGRESS) {
            std::cout << "Port " << port << " :CLOSED"; 
        } else {
            std::cout << "Port " << port << " :NOTSURE"; 
        }



        close(fd);

    }
}