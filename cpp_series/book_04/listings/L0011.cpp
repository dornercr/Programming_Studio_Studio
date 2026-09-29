#include "check.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <stdexcept>

std::uint32_t mask(unsigned bit) {
    if (bit >= 32) throw std::out_of_range("bit index");
    return std::uint32_t{1} << bit;
}
int main() {
    std::uint32_t flags = 0;
    flags |= mask(2) | mask(7);
    CHECK((flags & mask(2)) != 0 && (flags & mask(1)) == 0);
    flags &= ~mask(2);
    CHECK(flags == mask(7));
    flags ^= mask(7);
    CHECK(flags == 0);
    CHECK(std::popcount(mask(0) | mask(31)) == 2);
    const std::uint32_t value = 0x01020304U;
    const auto bytes = std::bit_cast<std::array<std::byte, sizeof(value)>>(value);
    std::cout << "native bytes:";
    for (auto byte : bytes)
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0') << std::to_integer<unsigned>(byte);
    std::cout << std::dec << '\n';
    bool rejected = false;
    try { (void)mask(32); } catch (const std::out_of_range&) { rejected = true; }
    CHECK(rejected);
    std::cout << "PASS\n";
}
