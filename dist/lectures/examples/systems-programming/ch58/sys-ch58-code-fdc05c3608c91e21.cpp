#include <cassert>
#include <iostream>

int main() {
    const unsigned expected_guard = 0xA55Au;
    unsigned saved_guard = expected_guard;
    saved_guard ^= 1u; // Deliberately model corruption; no invalid memory access.
    std::cout << (saved_guard == expected_guard ? "guard intact" : "detected") << '\n';
    assert(saved_guard != expected_guard);
}
