#pragma once
#ifndef __linux__
#error "HarborRelay native adapter requires Linux"
#endif
#include <atomic>
#include <chrono>
#include <cstddef>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace harbor {
class Fd {
    int fd_ = -1;
public:
    explicit Fd(int fd = -1) noexcept : fd_(fd) {}
    ~Fd();
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;
    Fd(Fd&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}
    Fd& operator=(Fd&& other) noexcept;
    int get() const noexcept { return fd_; }
    int release() noexcept { return std::exchange(fd_, -1); }
    void reset(int fd = -1) noexcept;
};
enum class Failure { io, timeout, cancelled, truncated, oversize };
class TransportError : public std::runtime_error {
public:
    Failure category;
    int native_error;
    TransportError(Failure why, std::string what, int error = 0)
        : std::runtime_error(std::move(what)), category(why), native_error(error) {}
};
using Clock = std::chrono::steady_clock;
struct Operation {
    Clock::time_point deadline;
    const std::atomic<bool>* stop = nullptr;
    void check() const;
};
void nonblocking(int fd);
Fd listen_local(unsigned& selected_port);
Fd connect_local(unsigned port, const Operation& operation);
bool read_exact(int fd, std::span<std::byte> bytes, const Operation& operation);
void write_exact(int fd, std::span<const std::byte> bytes, const Operation& operation);
std::optional<std::string> receive_frame(int fd, const Operation& operation);
void send_frame(int fd, std::string_view payload, const Operation& operation);
} // namespace harbor
