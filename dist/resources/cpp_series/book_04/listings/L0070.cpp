#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

int main() {
    sockaddr_in endpoint{};
    endpoint.sin_family = AF_INET;
    endpoint.sin_port = htons(8080);
    ::inet_pton(AF_INET, "127.0.0.1", &endpoint.sin_addr);
    char text[INET_ADDRSTRLEN]{};
    ::inet_ntop(AF_INET,&endpoint.sin_addr,text,sizeof text);
    std::cout << text << ':' << ntohs(endpoint.sin_port) << " transport=TCP-or-UDP\n";
}
