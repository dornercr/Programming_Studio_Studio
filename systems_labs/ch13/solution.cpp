#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>

std::uint64_t write_register(std::uint64_t old, std::uint64_t value, unsigned width) {
    if(width == 64) return value;
    if(width == 32) return static_cast<std::uint32_t>(value);
    if(width == 16) return (old & ~std::uint64_t{0xFFFF}) | (value & 0xFFFFu);
    throw std::invalid_argument("unsupported width");
}

int main() {
    const std::uint64_t old = 0xFFFFFFFF00000000ULL;
    assert(write_register(old,7,32) == 7);
    assert(write_register(old,7,16) == 0xFFFFFFFF00000007ULL);
    assert(write_register(old,9,64) == 9);
    bool rejected = false;
    try { write_register(old,7,8); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << std::hex << write_register(old,7,32) << ' ' << write_register(old,7,16) << '\n';
}
