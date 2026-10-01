#include <iomanip>
#include <iostream>
int main() {
    const double compute = 20.0, other = 80.0;
    const double before = compute + other;
    const double after = compute / 2.0 + other;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "modeled before=" << before << " after=" << after << '\n';
    std::cout << "whole speedup=" << before / after << '\n';
}
