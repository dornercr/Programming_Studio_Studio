#include "native.hpp"
#include "frame.hpp"
#include <algorithm>
#include <array>
#include <cerrno>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace harbor {
Fd::~Fd() { reset(); }
Fd& Fd::operator=(Fd&& other) noexcept {
    if (this != &other) { reset(); fd_ = other.release(); }
    return *this;
}
void Fd::reset(int fd) noexcept {
    const int old = std::exchange(fd_, fd);
    if (old >= 0) {
        const int saved = errno;
        ::close(old); // Linux: never retry the released descriptor number
        errno = saved;
    }
}
namespace {
[[noreturn]] void io_error(const char* operation, int error = errno) {
    throw TransportError(Failure::io, std::string(operation) + " (errno " +
                         std::to_string(error) + ")", error);
}
void ready(int fd, short events, const Operation& operation) {
    for (;;) {
        operation.check();
        auto left = std::chrono::ceil<std::chrono::milliseconds>(
            operation.deadline - Clock::now());
        const auto ms = std::clamp<long long>(left.count(), 1, 25);
        pollfd item{fd, events, 0};
        const int result = ::poll(&item, 1, static_cast<int>(ms));
        if (result > 0) {
            if (item.revents & POLLNVAL) io_error("poll invalid fd", EBADF);
            operation.check();
            return; // actual I/O interprets readiness, EOF, HUP, or error
        }
        if (result < 0 && errno != EINTR) io_error("poll");
    }
}
Fd new_socket() {
    const int fd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if (fd < 0) io_error("socket");
    return Fd(fd);
}
}
void Operation::check() const {
    if (stop && stop->load(std::memory_order_acquire))
        throw TransportError(Failure::cancelled, "operation cancelled");
    if (Clock::now() >= deadline)
        throw TransportError(Failure::timeout, "operation deadline exceeded");
}
void nonblocking(int fd) {
    const int flags = ::fcntl(fd, F_GETFL);
    if (flags < 0 || ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        io_error("fcntl nonblocking");
}
Fd listen_local(unsigned& selected_port) {
    Fd socket = new_socket();
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (::bind(socket.get(), reinterpret_cast<sockaddr*>(&address), sizeof address) < 0)
        io_error("bind");
    if (::listen(socket.get(), 16) < 0) io_error("listen");
    socklen_t length = sizeof address;
    if (::getsockname(socket.get(), reinterpret_cast<sockaddr*>(&address), &length) < 0)
        io_error("getsockname");
    selected_port = ntohs(address.sin_port);
    return socket;
}
Fd connect_local(unsigned port, const Operation& operation) {
    if (port == 0 || port > 65535) throw std::invalid_argument("invalid port");
    operation.check();
    Fd socket = new_socket();
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(static_cast<std::uint16_t>(port));
    if (::connect(socket.get(), reinterpret_cast<sockaddr*>(&address), sizeof address) < 0) {
        if (errno != EINPROGRESS) io_error("connect");
        ready(socket.get(), POLLOUT, operation);
        int error = 0;
        socklen_t size = sizeof error;
        if (::getsockopt(socket.get(), SOL_SOCKET, SO_ERROR, &error, &size) < 0)
            io_error("getsockopt SO_ERROR");
        if (error != 0) io_error("connect completion", error);
    }
    operation.check();
    return socket;
}
bool read_exact(int fd, std::span<std::byte> bytes, const Operation& operation) {
    std::size_t offset = 0;
    while (offset < bytes.size()) {
        operation.check();
        const auto n = ::recv(fd, bytes.data() + offset, std::min<std::size_t>(bytes.size() - offset, 1u << 20), 0);
        if (n > 0) { offset += static_cast<std::size_t>(n); continue; }
        if (n == 0) {
            if (offset == 0) return false;
            throw TransportError(Failure::truncated, "EOF inside transfer");
        }
        const int error = errno;
        if (error == EINTR) continue;
        if (error == EAGAIN || error == EWOULDBLOCK) {
            ready(fd, POLLIN, operation); continue;
        }
        io_error("recv", error);
    }
    operation.check();
    return true;
}
void write_exact(int fd, std::span<const std::byte> bytes, const Operation& operation) {
    std::size_t offset = 0;
    while (offset < bytes.size()) {
        operation.check();
        const auto n = ::send(fd, bytes.data() + offset, std::min<std::size_t>(bytes.size() - offset, 1u << 20), MSG_NOSIGNAL);
        if (n > 0) { offset += static_cast<std::size_t>(n); continue; }
        if (n == 0) throw TransportError(Failure::io, "send made no progress");
        const int error = errno;
        if (error == EINTR) continue;
        if (error == EAGAIN || error == EWOULDBLOCK) {
            ready(fd, POLLOUT, operation); continue;
        }
        io_error("send", error);
    }
    operation.check();
}
std::optional<std::string> receive_frame(int fd, const Operation& operation) {
    std::array<std::byte, 4> header{};
    if (!read_exact(fd, header, operation)) return std::nullopt;
    std::size_t length = 0;
    try { length = decode_length(header); }
    catch (const std::length_error&) {
        throw TransportError(Failure::oversize, "oversized incoming frame");
    }
    std::string payload(length, '\0');
    auto bytes = std::as_writable_bytes(std::span(payload.data(), payload.size()));
    if (!read_exact(fd, bytes, operation))
        throw TransportError(Failure::truncated, "EOF before declared body");
    return payload;
}
void send_frame(int fd, std::string_view payload, const Operation& operation) {
    operation.check();
    auto encoded = encode_frame(payload);
    write_exact(fd, encoded, operation);
}
} // namespace harbor
