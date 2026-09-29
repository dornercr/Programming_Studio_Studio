#include "check.hpp"
#include "linux_support.hpp"
#include <algorithm>
#include <array>
#include <exception>
#include <string>
#include <thread>

int main() {
    using namespace harbor_linux;
    auto listener = listen_loopback();
    std::exception_ptr server_error;
    std::jthread server([&] {
        try {
            auto connection = accept_one(listener.socket.get());
            auto request = receive_frame(connection.get());
            if (!request) throw std::runtime_error("missing request");
            std::reverse(request->begin(), request->end());
            send_frame(connection.get(), *request);
        } catch (...) { server_error = std::current_exception(); }
    });
    auto client = connect_loopback(listener.port);
    const std::string request{"a\0bc", 4};
    send_frame(client.get(), request);
    const auto reply = receive_frame(client.get());
    server.join();
    if (server_error) std::rethrow_exception(server_error);
    CHECK(reply && *reply == std::string("cb\0a", 4));

    auto receiver = checked_fd(::socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0), "UDP socket");
    auto sender = checked_fd(::socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0), "UDP socket");
    socket_timeout(receiver.get()); socket_timeout(sender.get());
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (::bind(receiver.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0) fail("UDP bind");
    socklen_t address_size = sizeof(address);
    if (::getsockname(receiver.get(), reinterpret_cast<sockaddr*>(&address), &address_size) < 0) fail("UDP name");
    const std::string datagram = "sequence=1";
    const auto sent = ::sendto(sender.get(), datagram.data(), datagram.size(), 0,
        reinterpret_cast<const sockaddr*>(&address), sizeof(address));
    CHECK(sent == static_cast<ssize_t>(datagram.size()));
    std::array<char, 64> buffer{};
    sockaddr_in peer{}; socklen_t peer_size = sizeof(peer);
    const auto received = ::recvfrom(receiver.get(), buffer.data(), buffer.size(), MSG_TRUNC,
        reinterpret_cast<sockaddr*>(&peer), &peer_size);
    CHECK(received >= 0 && static_cast<std::size_t>(received) <= buffer.size());
    CHECK(std::string(buffer.data(), static_cast<std::size_t>(received)) == datagram);
    CHECK(peer.sin_family == AF_INET && peer.sin_addr.s_addr == htonl(INADDR_LOOPBACK));
    std::cout << "PASS\n";
}
