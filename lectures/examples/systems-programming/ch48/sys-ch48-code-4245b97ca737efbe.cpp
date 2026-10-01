// LAB: Use a condition variable with a lasting predicate
// Implement a one-shot handoff using a mutex, condition variable, ready flag, and payload. It must work whether the producer sets ready before or after the consumer starts waiting.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> queue;
    queue.push_back(7); // The producer acts before the consumer arrives.
    const bool should_wait = queue.empty();
    std::cout << "wait=" << std::boolalpha << should_wait
              << " value=" << queue.front() << '\n';
    assert(!should_wait);
}
