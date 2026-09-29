#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    std::uint16_t value = 0x2A3C;
    std::cout << std::dec << value << "\n";
    std::cout << "0x" << std::hex << value << "\n";
    std::cout << std::bitset<16>(value) << "\n";
    auto* bytes = reinterpret_cast<unsigned char*>(&value);
    std::cout << std::hex << +bytes[0] << ' ' << +bytes[1] << "\n";
}
