#include <iostream>
#include <array>
#include <cstdint>

std::uint32_t readLE32(const std::array<unsigned char,4>& bytes) {
    // TODO: combine all four bytes in little-endian order.
    return bytes[0];
}
