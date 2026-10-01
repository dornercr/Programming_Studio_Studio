#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

int main() {
    int payload = 0;
    std::atomic<bool> ready{false};
    int observed = 0;
    std::thread producer([&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) std::this_thread::yield();
        observed = payload;
    });
    producer.join();
    consumer.join();
    std::cout << observed << '\n';
    assert(observed == 42);
}
