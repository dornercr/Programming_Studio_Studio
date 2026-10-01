#include <array>
#include <cstddef>
#include <iostream>
int main() {
    const std::array<std::size_t, 2> jump_to{1, 0};
    std::size_t pc = 0, steps = 0;
    constexpr std::size_t budget = 5;
    while (steps < budget && pc < jump_to.size()) {
        pc = jump_to[pc];
        ++steps;
    }
    std::cout << "steps=" << steps << " pc=" << pc
              << " halted=false\n";
}
