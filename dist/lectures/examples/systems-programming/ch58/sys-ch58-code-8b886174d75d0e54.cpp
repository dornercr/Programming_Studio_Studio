// LAB: Show both detection and its limit
// Model a record with payload and guard. Demonstrate that changing the guard is detected while a payload-only change can leave the guard check satisfied. Do not perform an invalid memory access.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const unsigned expected_guard = 0xA55Au;
    unsigned saved_guard = expected_guard;
    saved_guard ^= 1u; // Deliberately model corruption; no invalid memory access.
    std::cout << (saved_guard == expected_guard ? "guard intact" : "detected") << '\n';
    assert(saved_guard != expected_guard);
}
