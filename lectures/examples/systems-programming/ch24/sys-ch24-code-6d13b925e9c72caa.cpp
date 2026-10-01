#include <iostream>
int main() {
    constexpr unsigned address = 0x6B;
    constexpr unsigned offset = address & 0xF;
    constexpr unsigned leaf = (address >> 4) & 0x3;
    constexpr unsigned root = (address >> 6) & 0x3;
    const unsigned rebuilt = (root << 6) | (leaf << 4) | offset;
    std::cout << "root=" << root << " leaf=" << leaf << " offset=" << offset << '\n';
    std::cout << "rebuilt=" << rebuilt << '\n';
}
