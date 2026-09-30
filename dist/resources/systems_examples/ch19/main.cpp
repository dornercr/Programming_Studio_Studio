#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const auto kernel_copy = [](std::size_t length) {
        return length <= 8 ? "accepted" : "rejected";
    };
    std::cout << kernel_copy(4) << ' ' << kernel_copy(12) << '\n';
}
