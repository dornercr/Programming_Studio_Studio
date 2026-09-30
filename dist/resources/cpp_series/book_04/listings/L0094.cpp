#include "check.hpp"
#include "bounded_queue.hpp"
#include <exception>
#include <numeric>
#include <thread>
#include <vector>

int main() {
    BoundedQueue<int> queue{3};
    std::vector<int> received;
    std::exception_ptr producer_error, consumer_error;
    std::vector<std::jthread> workers;
    struct CloseFirst { BoundedQueue<int>& queue; ~CloseFirst() { queue.close(); } };
    CloseFirst startup_guard{queue}; // Destroyed before workers on failure.
    workers.emplace_back([&] {
        try { while (auto item = queue.pop()) received.push_back(*item); }
        catch (...) { consumer_error = std::current_exception(); queue.close(); }
    });
    workers.emplace_back([&] {
        try {
            for (int value = 0; value < 100; ++value)
                if (!queue.push(value)) throw std::runtime_error("unexpected closure");
        } catch (...) { producer_error = std::current_exception(); }
        queue.close();
    });
    workers.clear();
    if (producer_error) std::rethrow_exception(producer_error);
    if (consumer_error) std::rethrow_exception(consumer_error);
    std::vector<int> expected(100); std::iota(expected.begin(), expected.end(), 0);
    CHECK(received == expected);
    CHECK(!queue.push(101) && !queue.pop());
    queue.close(); // Repeated closure is harmless.
    std::cout << "PASS\n";
}
