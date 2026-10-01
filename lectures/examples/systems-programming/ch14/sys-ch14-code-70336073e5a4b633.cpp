#include <array>
#include <cstddef>
#include <iostream>
int main() {
    std::array<int, 32> memory{};
    memory[26] = 99;
    const std::size_t x = 7;
    const auto calculated = 5 + x + 2 * x; // Address-form arithmetic only.
    const int loaded = memory.at(calculated); // Separate checked access.
    std::cout << "calculated=" << calculated << " loaded=" << loaded << '\n';
}
