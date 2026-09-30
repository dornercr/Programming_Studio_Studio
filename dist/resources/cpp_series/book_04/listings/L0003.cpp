#include <cstdint>
#include <iostream>

std::uint64_t transform(std::uint64_t x) {
    return (x + 5) * 4;
}

int main() {
    std::cout << transform(7) << "\n";
}
