#include "bounded_queue.hpp"
#include "check.hpp"
#include <algorithm>
#include <array>
#include <atomic>
#include <exception>
#include <iostream>
#include <thread>
#include <vector>

namespace {
struct CloseQueue {
    BoundedQueue<int>& queue;
    ~CloseQueue() { queue.close(); }
};
void partial_startup(std::atomic<bool>& finished) {
    BoundedQueue<int> queue(1);
    std::vector<std::jthread> workers;
    CloseQueue cleanup{queue}; // closes before workers join on exception
    workers.emplace_back([&] { while (queue.pop()) {} finished.store(true); });
    throw std::runtime_error("injected next-thread startup failure");
}
}
int main() {
    try {
        BoundedQueue<int> queue(3);
        std::array<std::vector<int>, 3> outputs;
        std::array<std::exception_ptr, 5> errors;
        std::vector<std::jthread> consumers, producers;
        CloseQueue cleanup{queue};
        for (std::size_t c = 0; c < outputs.size(); ++c) consumers.emplace_back([&, c] {
            try { while (auto item = queue.pop()) outputs[c].push_back(*item); }
            catch (...) { errors[c] = std::current_exception(); queue.close(); }
        });
        for (int p = 0; p < 2; ++p) producers.emplace_back([&, p] {
            try { for (int i = 0; i < 500; ++i)
                if (!queue.push(p * 500 + i)) throw std::runtime_error("unexpected closure"); }
            catch (...) { errors[3 + p] = std::current_exception(); queue.close(); }
        });
        producers.clear(); queue.close(); consumers.clear();
        for (auto error : errors) if (error) std::rethrow_exception(error);
        std::vector<int> collected;
        for (auto& values : outputs) collected.insert(collected.end(), values.begin(), values.end());
        std::sort(collected.begin(), collected.end());
        CHECK(collected.size() == 1000);
        for (int i = 0; i < 1000; ++i) CHECK(collected[static_cast<std::size_t>(i)] == i);
        CHECK(!queue.push(1001)); CHECK(!queue.pop());
        std::atomic<bool> finished{false};
        bool injected = false;
        try { partial_startup(finished); } catch (const std::runtime_error&) { injected = true; }
        CHECK(injected); CHECK(finished.load());
        std::cout << "PASS 1000 unique accepted items and partial-startup cleanup\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
