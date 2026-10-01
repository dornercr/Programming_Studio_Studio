#include <iomanip>
#include <iostream>
int main() {
    const double l1_lookup = 1.0;
    const double l1_miss = 0.10;
    const double l2_lookup = 6.0;
    const double l2_local_miss = 0.25;
    const double memory_extra = 80.0;
    const double after_l1_miss = l2_lookup + l2_local_miss * memory_extra;
    const double average = l1_lookup + l1_miss * after_l1_miss;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "L1 miss path=" << after_l1_miss << " ns\n";
    std::cout << "modeled AMAT=" << average << " ns\n";
}
