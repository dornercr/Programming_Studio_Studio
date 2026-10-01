#include <array>
#include <iostream>
#include <string_view>
std::string_view dispatch(int selector) {
    constexpr std::array<std::string_view, 3> names{"stop", "run", "wait"};
    if (selector < 10 || selector > 12) return "default";
    return names[static_cast<unsigned>(selector - 10)];
}
int main() {
    for (int selector : {9, 10, 12, 13}) {
        std::cout << selector << ':' << dispatch(selector) << '\n';
    }
}
