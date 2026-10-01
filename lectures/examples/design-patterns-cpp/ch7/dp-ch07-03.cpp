#include <iostream>
#include <string>

std::string plain_warning(int n) {
    // Updated requirement: only values above ten are high.
    return std::string(n > 10 ? "high=" : "normal=") + std::to_string(n);
}
std::string bracket_warning(int n) {
    // Deliberate bug: this copy still uses the old boundary.
    return std::string(n > 5 ? "[high:" : "[normal:") + std::to_string(n) + "]";
}
int main() {
    const int observation = 6; // This demonstration uses valid fixed input.
    std::cout << plain_warning(observation) << '\n';
    std::cout << bracket_warning(observation) << '\n';
}
