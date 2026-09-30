#include "check.hpp"
#include <atomic>
#include <exception>
#include <iostream>
#include <optional>
#include <thread>
#include <vector>

namespace {
// Exactly one producer and one consumer. State 0 grants the producer access;
// state 1 grants the consumer access; state 2 is terminal. close never reopens.
class Handoff {
    std::atomic<unsigned> state_{0};
    int payload_ = 0;
public:
    bool publish(int value) {
        unsigned state = state_.load(std::memory_order_acquire);
        while (state == 1) {
            state_.wait(1, std::memory_order_acquire);
            state = state_.load(std::memory_order_acquire);
        }
        if (state == 2) return false;
        payload_ = value; // consumer's prior release granted reuse
        unsigned expected = 0;
        if (!state_.compare_exchange_strong(expected, 1, std::memory_order_release,
                                            std::memory_order_relaxed)) return false;
        state_.notify_one();
        return true;
    }
    std::optional<int> consume() {
        unsigned state = state_.load(std::memory_order_acquire);
        while (state == 0) {
            state_.wait(0, std::memory_order_acquire);
            state = state_.load(std::memory_order_acquire);
        }
        if (state == 2) return std::nullopt;
        const int value = payload_;
        unsigned expected = 1;
        state_.compare_exchange_strong(expected, 0, std::memory_order_release,
                                       std::memory_order_relaxed);
        state_.notify_one(); // CAS cannot overwrite terminal state 2
        return value;
    }
    void close() noexcept {
        state_.store(2, std::memory_order_release);
        state_.notify_all();
    }
};
struct Close { Handoff& channel; ~Close() { channel.close(); } };
void injected_failure() {
    Handoff channel;
    std::vector<std::jthread> workers;
    Close cleanup{channel};
    workers.emplace_back([&] { for (int i = 0; i < 5000; ++i)
        if (!channel.publish(i)) break; });
    throw std::runtime_error("injected missing consumer");
}
}
int main() {
    try {
        Handoff channel;
        std::vector<int> received;
        received.reserve(5000); // bounded, nonthrowing push for these int values
        std::vector<std::jthread> workers;
        Close cleanup{channel};
        workers.emplace_back([&] { for (int i = 0; i < 5000; ++i)
            if (!channel.publish(i)) break; });
        workers.emplace_back([&] { for (int i = 0; i < 5000; ++i) {
            auto value = channel.consume();
            if (!value) break;
            received.push_back(*value);
        }});
        workers.clear();
        CHECK(received.size() == 5000);
        for (int i = 0; i < 5000; ++i) CHECK(received[static_cast<std::size_t>(i)] == i);
        channel.close(); CHECK(!channel.publish(9)); CHECK(!channel.consume());
        bool injected = false;
        try { injected_failure(); } catch (const std::runtime_error&) { injected = true; }
        CHECK(injected);
        std::cout << "PASS 5000 two-way ownership handoffs and cancelled startup\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
