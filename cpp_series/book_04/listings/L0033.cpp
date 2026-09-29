#include <cstdint>
#include <cstring>
#include <iostream>

int main() {
    std::uint8_t buffer[8]{};
    std::uint32_t value = 0x11223344;
    std::memcpy(buffer + 2, &value, sizeof value);
    std::uint32_t decoded{};
    std::memcpy(&decoded, buffer + 2, sizeof decoded);
    std::cout << std::hex << decoded << "\n";
}
