#include <cstddef>
#include <iostream>
std::size_t pages(std::size_t bytes) {
    constexpr std::size_t size = 8;
    return bytes/size + (bytes%size != 0);
}
int main() {
    // Region begins at a page boundary in this arithmetic model.
    for (std::size_t bytes : {0u,1u,8u,9u})
        std::cout << "bytes=" << bytes << " floor=" << bytes/8
                  << " pages=" << pages(bytes) << '\n';
}
