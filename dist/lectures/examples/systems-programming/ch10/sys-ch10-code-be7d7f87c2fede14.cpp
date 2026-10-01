#include <array>
#include <cstddef>
#include <iostream>
int main() {
    const std::array<std::size_t, 3> free_sizes{30, 18, 24};
    const std::size_t request = 16;
    std::size_t first = free_sizes.size();
    std::size_t best = free_sizes.size();
    for (std::size_t i = 0; i < free_sizes.size(); ++i) {
        if (free_sizes[i] < request) continue;
        if (first == free_sizes.size()) first = i;
        if (best == free_sizes.size() || free_sizes[i] < free_sizes[best]) best = i;
    }
    if (first == free_sizes.size() || best == free_sizes.size()) return 1;
    std::cout << "first fit=" << free_sizes[first]
              << " remainder=" << free_sizes[first]-request << '\n';
    std::cout << "best fit=" << free_sizes[best]
              << " remainder=" << free_sizes[best]-request << '\n';
}
