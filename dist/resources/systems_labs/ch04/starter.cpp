// LAB: Return a checked byte count
// Write checked_bytes(count,width,limit). Return no value when multiplication would exceed limit; zero-width requests return zero.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const std::size_t limit = 100;
    const std::size_t count = 9;
    const std::size_t width = 12;
    const bool fits = width == 0 || count <= limit / width;
    std::cout << (fits ? "fits" : "reject") << '\n';
    assert(!fits);
}
