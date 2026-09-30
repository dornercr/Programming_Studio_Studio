#ifndef HARBOR_SERIES_LINUX_SUPPORT_HPP
#define HARBOR_SERIES_LINUX_SUPPORT_HPP
// Linux-only teaching support. Sockets bind/connect only to IPv4 loopback.
#include <array>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

namespace harbor_linux {
[[noreturn]] inline void fail(const char* operation) {
    const int error = errno;
    throw std::system_error(error, std::generic_category(), operation);
}
class Fd {
    int value_{-1};
public:
    explicit Fd(int value = -1) noexcept : value_(value) {}
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;
    Fd(Fd&& other) noexcept : value_(other.release()) {}
    Fd& operator=(Fd&& other) noexcept {
        if (this != &other) { reset(); value_ = other.release(); }
        return *this;
    }
    ~Fd() { reset(); }
    int get() const noexcept { return value_; }
    explicit operator bool() const noexcept { return value_ >= 0; }
    int release() noexcept { return std::exchange(value_, -1); }
    void reset() noexcept {
        const int saved = errno;
        if (value_ >= 0) (void)::close(release());
        errno = saved;
    }
};
inline Fd checked_fd(int value, const char* operation) {
    if (value < 0) fail(operation);
    return Fd{value};
}
inline void write_all(int fd, std::span<const std::byte> data) {
    while (!data.empty()) {
        const auto count = ::write(fd, data.data(), data.size());
        if (count < 0) { if (errno == EINTR) continue; fail("write"); }
        if (count == 0) throw std::runtime_error("write made no progress");
        data = data.subspan(static_cast<std::size_t>(count));
    }
}
// false: clean EOF before any requested byte; partial EOF is a framing error.
inline bool read_exact(int fd, std::span<std::byte> data) {
    const auto original = data.size();
    while (!data.empty()) {
        const auto count = ::read(fd, data.data(), data.size());
        if (count < 0) { if (errno == EINTR) continue; fail("read"); }
        if (count == 0) {
            if (data.size() == original) return false;
            throw std::runtime_error("truncated input");
        }
        data = data.subspan(static_cast<std::size_t>(count));
    }
    return true;
}
inline void socket_timeout(int fd) {
    const timeval timeout{5, 0};
    if (::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) fail("receive timeout");
    if (::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) fail("send timeout");
}
inline void wait_readable(int fd, std::chrono::milliseconds duration = std::chrono::seconds{5}) {
    const auto deadline = std::chrono::steady_clock::now() + duration;
    for (;;) {
        const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
        if (remaining.count() <= 0) throw std::runtime_error("readiness timeout");
        pollfd item{fd, POLLIN, 0};
        const int result = ::poll(&item, 1, static_cast<int>(remaining.count()));
        if (result < 0) { if (errno == EINTR) continue; fail("poll"); }
        if (result == 0) throw std::runtime_error("readiness timeout");
        if (item.revents & POLLNVAL) throw std::runtime_error("invalid descriptor in poll");
        if (item.revents & (POLLIN | POLLHUP | POLLERR)) return;
    }
}
inline void send_all(int fd, std::span<const std::byte> data) {
    while (!data.empty()) {
        const auto count = ::send(fd, data.data(), data.size(), MSG_NOSIGNAL);
        if (count < 0) { if (errno == EINTR) continue; fail("send"); }
        if (count == 0) throw std::runtime_error("send made no progress");
        data = data.subspan(static_cast<std::size_t>(count));
    }
}
inline void send_frame(int fd, std::string_view text) {
    if (text.size() > 4096) throw std::length_error("frame exceeds 4096 bytes");
    const auto length = static_cast<std::uint32_t>(text.size());
    const std::array<std::byte, 4> header{
        std::byte((length >> 24) & 255U), std::byte((length >> 16) & 255U),
        std::byte((length >> 8) & 255U), std::byte(length & 255U)};
    send_all(fd, header);
    send_all(fd, std::as_bytes(std::span{text.data(), text.size()}));
}
inline std::optional<std::string> receive_frame(int fd) {
    std::array<std::byte, 4> header{};
    if (!read_exact(fd, header)) return std::nullopt;
    std::uint32_t length = 0;
    for (const auto byte : header) length = (length << 8) | std::to_integer<unsigned char>(byte);
    if (length > 4096) throw std::length_error("frame exceeds 4096 bytes");
    std::string result(length, '\0');
    if (!read_exact(fd, std::as_writable_bytes(std::span{result.data(), result.size()})))
        throw std::runtime_error("missing frame body");
    return result;
}
struct Listener { Fd socket; std::uint16_t port; };
inline Listener listen_loopback() {
    auto socket = checked_fd(::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0), "socket");
    socket_timeout(socket.get());
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (::bind(socket.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0) fail("bind");
    if (::listen(socket.get(), 16) < 0) fail("listen");
    socklen_t size = sizeof(address);
    if (::getsockname(socket.get(), reinterpret_cast<sockaddr*>(&address), &size) < 0) fail("getsockname");
    return {std::move(socket), ntohs(address.sin_port)};
}
inline Fd accept_one(int listener) {
    wait_readable(listener);
    const int raw = ::accept4(listener, nullptr, nullptr, SOCK_CLOEXEC);
    auto accepted = checked_fd(raw, "accept4");
    socket_timeout(accepted.get());
    return accepted;
}
inline Fd connect_loopback(std::uint16_t port) {
    auto socket = checked_fd(::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0), "socket");
    socket_timeout(socket.get());
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);
    if (::connect(socket.get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0) fail("connect");
    return socket;
}
}
#endif
