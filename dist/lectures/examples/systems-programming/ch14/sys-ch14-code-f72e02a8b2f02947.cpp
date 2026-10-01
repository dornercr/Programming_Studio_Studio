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
