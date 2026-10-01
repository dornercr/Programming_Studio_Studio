#include <cstddef>
#include <iostream>
#include <limits>
int main() {
    const std::size_t extent = 8;
    const auto offset = std::numeric_limits<std::size_t>::max() - 2;
    const std::size_t length = 5;
    const bool naive = offset + length <= extent;
    const bool guarded = offset <= extent && length <= extent - offset;
    std::cout << std::boolalpha << "naive=" << naive
              << " guarded=" << guarded << '\n';
}
