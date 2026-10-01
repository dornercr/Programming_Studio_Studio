#include <iomanip>
#include <iostream>
#include <limits>
int main() {
    static_assert(std::numeric_limits<double>::radix == 2);
    const double first = 0.625; // 1/2 + 1/8.
    const double second = 0.125; // 1/8.
    const double sum = first + second;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "first=" << first << '\n';
    std::cout << "sum=" << sum << '\n';
    std::cout << std::boolalpha << "exact three quarters="
              << (sum == 0.75) << '\n';
}
