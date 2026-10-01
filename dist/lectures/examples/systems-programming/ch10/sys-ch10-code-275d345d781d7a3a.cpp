#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const std::size_t capacity = 32;
    std::size_t used = 0;
    for (std::size_t request : {5u, 9u}) {
        const std::size_t start = (used + 7) / 8 * 8;
        assert(start <= capacity && request <= capacity - start);
        std::cout << start << ' ';
        used = start + request;
    }
    std::cout << "used=" << used << '\n';
}
