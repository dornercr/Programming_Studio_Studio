#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int main() {
    addrinfo hints{}; hints.ai_family=AF_UNSPEC; hints.ai_socktype=SOCK_STREAM;
    addrinfo* result=nullptr;
    int rc=::getaddrinfo("localhost","80",&hints,&result);
    if(rc!=0) return 1;
    int fd=::socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    std::cout << "family=" << result->ai_family << " socket_ok=" << (fd>=0) << "\n";
    if(fd>=0) ::close(fd); ::freeaddrinfo(result);
}
