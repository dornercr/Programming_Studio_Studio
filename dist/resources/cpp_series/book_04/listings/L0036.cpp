#include "check.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <span>
#include <stdexcept>

std::uint32_t little_u32(std::span<const std::byte> bytes) {
    if (bytes.size() < 4) throw std::invalid_argument("truncated field");
    std::uint32_t result = 0;
    for (unsigned i = 0; i < 4; ++i)
        result |= std::uint32_t{std::to_integer<unsigned char>(bytes[i])} << (8 * i);
    return result;
}
int square(int value) { return value * value; }
struct Sensor {
    int reading;
    int doubled() const { return reading * 2; }
};
int main() {
    const std::array<std::byte, 4> encoded{std::byte{0x04}, std::byte{0x03}, std::byte{0x02}, std::byte{0x01}};
    CHECK(little_u32(encoded) == 0x01020304U);
    bool rejected = false;
    try { (void)little_u32(std::span{encoded}.first<3>()); }
    catch (const std::invalid_argument&) { rejected = true; }
    CHECK(rejected);
    int (*operation)(int) = &square;
    CHECK(operation && std::invoke(operation, 5) == 25);
    const Sensor sensor{7};
    int Sensor::* field = &Sensor::reading;
    int (Sensor::* method)() const = &Sensor::doubled;
    CHECK(sensor.*field == 7);
    CHECK(std::invoke(method, sensor) == 14);
    std::cout << "PASS\n";
}
