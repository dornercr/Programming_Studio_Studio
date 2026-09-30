#include <iostream>
#include <array>
#include <cstdint>

std::uint32_t readLE32(const std::array<unsigned char,4>& bytes) {
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1])<<8) |
           (std::uint32_t(bytes[2])<<16) | (std::uint32_t(bytes[3])<<24);
}
