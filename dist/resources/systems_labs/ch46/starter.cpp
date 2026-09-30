// LAB: Publish a caller-supplied payload
// Build a one-shot publication function. A worker writes a non-atomic payload then releases a ready flag; the caller acquires that flag before reading the payload.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
