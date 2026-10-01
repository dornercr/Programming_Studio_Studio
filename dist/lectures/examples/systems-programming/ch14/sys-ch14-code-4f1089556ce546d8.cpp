// LAB: Check an effective-address calculation
// Compute base+index*scale+displacement under a caller-supplied upper bound. The result is a numeric model, not a dereferenceable C++ pointer.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstdint>
#include <iostream>

int main() {
    const std::uint64_t base = 1000;
    const std::uint64_t index = 3;
    const std::uint64_t scale = 4;
    const std::uint64_t displacement = 8;
    const auto address = base + index * scale + displacement;
    std::cout << address << '\n';
    assert(address == 1020);
}
