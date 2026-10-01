#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
struct Region { std::size_t bytes; bool free; };
std::size_t largest_run(const std::array<Region, 3>& regions) {
    std::size_t run = 0, largest = 0;
    for (const auto region : regions) {
        run = region.free ? run + region.bytes : 0;
        largest = std::max(largest, run);
    }
    return largest;
}
int main() {
    // Model: these three 8-byte regions are physically consecutive.
    std::array<Region, 3> regions{{{8,true}, {8,false}, {8,true}}};
    std::cout << "free total=16 largest=" << largest_run(regions) << '\n';
    std::cout << std::boolalpha << "12-byte request fits="
              << (largest_run(regions) >= 12) << '\n';
    regions[1].free = true;
    std::cout << "after middle release largest=" << largest_run(regions) << '\n';
    std::cout << "12-byte request fits=" << (largest_run(regions) >= 12) << '\n';
}
