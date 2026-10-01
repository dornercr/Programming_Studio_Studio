#include <iomanip>
#include <iostream>
int main() {
    constexpr double accesses = 1000, l1_misses = 100, l2_misses = 20;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "L1-local=" << 100*l1_misses/accesses << "%\n";
    std::cout << "L2-local=" << 100*l2_misses/l1_misses << "%\n";
    std::cout << "global=" << 100*l2_misses/accesses << "%\n";
}
