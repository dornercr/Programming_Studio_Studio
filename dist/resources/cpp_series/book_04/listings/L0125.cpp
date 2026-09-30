#pragma once
#include "inbox.hpp"
#include "native.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <exception>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace harbor {
std::string process_request(std::string_view text);
struct Config {
    unsigned workers = 2;
    std::size_t queue_capacity = 4;
    std::chrono::milliseconds request_budget{1000};
    // Deterministic test seam only; the command-line program never enables it.
    std::optional<unsigned> fail_before_worker;
};
struct Stats {
    std::uint64_t accepted{}, admitted{}, rejected{}, completed{},
                  application_errors{}, failures{}, timeouts{}, cancelled{};
};
class RelayServer {
    struct Work { Fd socket; Clock::time_point deadline; };
    struct Counters {
        std::atomic<std::uint64_t> accepted{}, admitted{}, rejected{}, completed{},
            application_errors{}, failures{}, timeouts{}, cancelled{};
    } stats_;
    Config config_;
    Fd listener_;
    unsigned port_ = 0;
    std::atomic<bool> stop_{false};
    BoundedInbox<Work> inbox_;
    std::exception_ptr accept_failure_; // read only after joining acceptor
    std::vector<std::jthread> workers_;
    std::jthread acceptor_;
    void accept_loop() noexcept;
    void worker_loop() noexcept;
public:
    explicit RelayServer(Config config = {});
    ~RelayServer();
    RelayServer(const RelayServer&) = delete;
    RelayServer& operator=(const RelayServer&) = delete;
    unsigned port() const noexcept { return port_; }
    void request_stop();
    // Only the supervising thread calls stop_and_join/destruction.
    void stop_and_join();
    void rethrow_failure() const;
    Stats snapshot() const noexcept; // coherent accounting only after join
};
std::string request(unsigned port, std::string_view text,
                    std::chrono::milliseconds budget = std::chrono::seconds(3));
} // namespace harbor
