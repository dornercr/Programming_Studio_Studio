#include "relay.hpp"
#include "frame.hpp"
#include <cerrno>
#include <charconv>
#include <poll.h>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>

namespace harbor {
std::string process_request(std::string_view text) {
    if (text.size() > max_payload) return "ERR size";
    std::istringstream input{std::string(text)};
    input.imbue(std::locale::classic());
    std::string token;
    if (!(input >> token) || token != "SUM") return "ERR command";
    long long total = 0;
    unsigned count = 0;
    while (input >> token) {
        if (++count > 64) return "ERR count";
        int value = 0;
        auto [end, error] = std::from_chars(token.data(), token.data() + token.size(), value);
        if (error != std::errc{} || end != token.data() + token.size() ||
            value < -1000000 || value > 1000000) return "ERR value";
        total += value; // <= 64 * 1,000,000 in magnitude
    }
    if (count == 0) return "ERR empty";
    return "OK " + std::to_string(total);
}
RelayServer::RelayServer(Config config)
    : config_(config), inbox_(config.queue_capacity) {
    if (config.workers == 0 || config.workers > 16 ||
        config.queue_capacity > 64 || config.request_budget.count() < 20 ||
        config.request_budget.count() > 5000)
        throw std::invalid_argument("invalid server configuration");
    listener_ = listen_local(port_);
    try {
        workers_.reserve(config.workers);
        for (unsigned i = 0; i < config.workers; ++i) {
            if (config.fail_before_worker == i)
                throw std::runtime_error("injected worker startup failure");
            workers_.emplace_back([this] { worker_loop(); });
        }
        acceptor_ = std::jthread([this] { accept_loop(); });
    } catch (...) {
        request_stop();
        workers_.clear(); // shared state remains alive; blocked pops wake
        throw;
    }
}
RelayServer::~RelayServer() { stop_and_join(); }
void RelayServer::request_stop() {
    stop_.store(true, std::memory_order_release);
    inbox_.close();
}
void RelayServer::stop_and_join() {
    request_stop();
    if (acceptor_.joinable()) acceptor_.join();
    workers_.clear();
    listener_.reset();
}
void RelayServer::rethrow_failure() const {
    if (accept_failure_) std::rethrow_exception(accept_failure_);
}
void RelayServer::accept_loop() noexcept {
    try {
        while (!stop_.load(std::memory_order_acquire)) {
            const int raw = ::accept4(listener_.get(), nullptr, nullptr,
                                      SOCK_CLOEXEC | SOCK_NONBLOCK);
            if (raw >= 0) {
                ++stats_.accepted;
                Work work{Fd(raw), Clock::now() + config_.request_budget};
                try {
                    if (inbox_.try_push(std::move(work))) ++stats_.admitted;
                    else ++stats_.rejected;
                } catch (...) { ++stats_.rejected; throw; }
                continue;
            }
            const int error = errno;
            if (error == EINTR) continue;
            if (error != EAGAIN && error != EWOULDBLOCK)
                throw TransportError(Failure::io, "accept failed", error);
            pollfd item{listener_.get(), POLLIN, 0};
            const int result = ::poll(&item, 1, 25);
            if (result < 0 && errno != EINTR)
                throw TransportError(Failure::io, "listener poll failed", errno);
            if (result > 0 && (item.revents & (POLLERR | POLLNVAL | POLLHUP)))
                throw TransportError(Failure::io, "listener became unusable");
        }
    } catch (...) {
        accept_failure_ = std::current_exception();
        request_stop();
    }
    inbox_.close();
}
void RelayServer::worker_loop() noexcept {
    // Queue operations have no user callbacks; fatal synchronization failures
    // are not recoverable application errors. Per-connection errors are contained.
    while (auto work = inbox_.pop()) {
        try {
            Operation op{work->deadline, &stop_};
            auto text = receive_frame(work->socket.get(), op);
            if (!text) throw TransportError(Failure::truncated, "no request frame");
            auto response = process_request(*text);
            send_frame(work->socket.get(), response, op);
            if (response.starts_with("ERR ")) ++stats_.application_errors;
            ++stats_.completed;
        } catch (const TransportError& error) {
            if (error.category == Failure::cancelled) ++stats_.cancelled;
            else {
                if (error.category == Failure::timeout) ++stats_.timeouts;
                ++stats_.failures;
            }
        } catch (...) { ++stats_.failures; }
    }
}
Stats RelayServer::snapshot() const noexcept {
    return {stats_.accepted.load(), stats_.admitted.load(), stats_.rejected.load(),
            stats_.completed.load(), stats_.application_errors.load(),
            stats_.failures.load(), stats_.timeouts.load(), stats_.cancelled.load()};
}
std::string request(unsigned port, std::string_view text,
                    std::chrono::milliseconds budget) {
    if (budget.count() < 1 || budget.count() > 30000)
        throw std::invalid_argument("invalid client budget");
    Operation op{Clock::now() + budget};
    auto socket = connect_local(port, op);
    send_frame(socket.get(), text, op);
    auto response = receive_frame(socket.get(), op);
    if (!response) throw TransportError(Failure::truncated, "no response frame");
    return *response;
}
} // namespace harbor
