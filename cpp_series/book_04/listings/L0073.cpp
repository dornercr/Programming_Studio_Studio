#include "check.hpp"
#include "linux_support.hpp"
#include <array>
#include <cstring>
#include <memory>
#include <netdb.h>
#include <string>

int main() {
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;
    addrinfo* raw = nullptr;
    const int error = ::getaddrinfo("127.0.0.1", "8080", &hints, &raw);
    if (error != 0) throw std::runtime_error(::gai_strerror(error));
    std::unique_ptr<addrinfo, decltype(&::freeaddrinfo)> addresses{raw, ::freeaddrinfo};
    CHECK(addresses && addresses->ai_family == AF_INET);
    std::array<char, NI_MAXHOST> host{};
    std::array<char, NI_MAXSERV> service{};
    const int named = ::getnameinfo(addresses->ai_addr, addresses->ai_addrlen,
        host.data(), host.size(), service.data(), service.size(), NI_NUMERICHOST | NI_NUMERICSERV);
    CHECK(named == 0);
    CHECK(std::string{host.data()} == "127.0.0.1" && std::string{service.data()} == "8080");
    auto listener = harbor_linux::listen_loopback();
    CHECK(listener.port != 0);
    int type{}; socklen_t size = sizeof(type);
    CHECK(::getsockopt(listener.socket.get(), SOL_SOCKET, SO_TYPE, &type, &size) == 0);
    CHECK(type == SOCK_STREAM);
    std::cout << "loopback_port=" << listener.port << "\nPASS\n";
}
