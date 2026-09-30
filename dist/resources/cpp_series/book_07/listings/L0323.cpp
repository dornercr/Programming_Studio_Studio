#pragma once
#include <chrono>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
namespace harbor {
using Steady = std::chrono::steady_clock;
class Fd {
    int value_=-1;
public:
    explicit Fd(int value=-1): value_(value) {}
    ~Fd();
    Fd(const Fd&)=delete;
    Fd& operator=(const Fd&)=delete;
    Fd(Fd&& other) noexcept;
    Fd& operator=(Fd&& other) noexcept;
    int get() const { return value_; }
    int release() noexcept;
    void reset(int value=-1) noexcept;
};
std::uint16_t port_number(std::string_view text, bool allow_zero=false);
std::string exchange(std::uint16_t port, std::string_view request,
                     std::chrono::milliseconds timeout);
class Server {
    Fd listener_;
    std::uint16_t port_{};
public:
    using Handler = std::function<std::optional<std::string>(std::string_view)>;
    explicit Server(std::uint16_t port);
    std::uint16_t port() const { return port_; }
    // IPv4 loopback only. One bounded frame and response per connection.
    // Handler must be bounded: poll cannot preempt a blocking callback.
    void run(const Handler& handler, const std::function<bool()>& stopping);
};
}
