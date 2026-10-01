#include <cstdint>
#include <iostream>
int main() {
    const std::uint64_t whole = 0x1234;
    const auto low = whole & 0xffu;
    const auto next = (whole >> 8) & 0xffu;
    const auto after_byte = (whole & ~std::uint64_t{0xff}) | 0xabu;
    std::cout << "low=" << low << " next=" << next << '\n';
    std::cout << std::hex << "after-byte=" << after_byte << '\n';
}
