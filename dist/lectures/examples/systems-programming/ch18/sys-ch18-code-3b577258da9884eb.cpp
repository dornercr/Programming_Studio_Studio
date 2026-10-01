#include <array>
#include <cstddef>
#include <iostream>
void show(const char* label, const std::array<int, 5>& a) {
    std::cout << label;
    for (int value : a) std::cout << ' ' << value;
    std::cout << '\n';
}
int main() {
    std::array<int, 5> forward{1, 2, 3, 4, 5};
    auto backward = forward;
    for (std::size_t i = 0; i < 4; ++i) forward[i + 1] = forward[i];
    for (std::size_t i = 4; i > 0; --i) backward[i] = backward[i - 1];
    show("forward:", forward);
    show("backward:", backward);
}
