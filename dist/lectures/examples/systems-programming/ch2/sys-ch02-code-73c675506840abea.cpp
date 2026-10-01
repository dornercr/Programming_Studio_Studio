#include <cstddef>
#include <iostream>
int main() {
    const std::ptrdiff_t result = -1; // A modeled error status.
    const auto premature = static_cast<std::size_t>(result);
    std::cout << std::boolalpha << "converted looks large="
              << (premature > 100) << '\n';
    if (result < 0) {
        std::cout << "validated=error\n";
    } else {
        const auto count = static_cast<std::size_t>(result);
        std::cout << "validated count=" << count << '\n';
    }
}
