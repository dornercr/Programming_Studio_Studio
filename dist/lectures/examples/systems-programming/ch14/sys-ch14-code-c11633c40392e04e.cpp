#include <array>
#include <climits>
#include <cstdint>
#include <iostream>
int main() {
    static_assert(CHAR_BIT == 8);
    const std::array<std::uint16_t, 4> values{3, 7, 11, 15};
    const auto* selected = values.data() + 2;
    const auto elements = selected - values.data();
    std::cout << "elements=" << elements
              << " bytes=" << elements * sizeof(values[0])
              << " value=" << *selected << '\n';
}
