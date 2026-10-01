#include <array>
#include <iostream>
int main() {
    const std::array<unsigned char, 2> bytes{0x02, 0x01};
    const unsigned big = static_cast<unsigned>(bytes[0]) * 256u + bytes[1];
    const unsigned little = static_cast<unsigned>(bytes[1]) * 256u + bytes[0];
    std::cout << "big endian=" << big << '\n';
    std::cout << "little endian=" << little << '\n';
    std::cout << "first stored octet=" << static_cast<unsigned>(bytes[0]) << '\n';
}
