#include <cstdint>
#include <iostream>

int main() {
    constexpr std::uint8_t Ready = 1u << 0;
    constexpr std::uint8_t Error = 1u << 2;
    std::uint8_t flags = 0;
    flags |= Ready;             // set bit 0
    flags |= Error;             // set bit 2
    flags &= static_cast<std::uint8_t>(~Error); // clear bit 2
    flags ^= Ready;             // toggle bit 0
    std::cout << ((flags & Ready) != 0) << "\n";
}
