#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
int main() {
    const std::uint8_t bits = std::numeric_limits<std::uint8_t>::max();
    const auto signed_value = std::bit_cast<std::int8_t>(bits);
    std::cout << std::boolalpha
              << "signed-less=" << (signed_value < 1)
              << " unsigned-less=" << (bits < 1u) << '\n';
}
