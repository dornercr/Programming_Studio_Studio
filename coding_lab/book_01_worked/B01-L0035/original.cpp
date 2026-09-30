#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    constexpr int max_devices{1000};
    const int count{12};
    const bool within_limit = count <= max_devices;
    const unsigned int wrapped =
        std::numeric_limits<unsigned int>::max() + 1U;
    const double sum = 0.1 + 0.2;
    std::cout << std::boolalpha << within_limit << '\n';
    std::cout << "unsigned wrap=" << wrapped << '\n';
    std::cout << std::setprecision(17) << "sum=" << sum << '\n';
    if (!within_limit || wrapped != 0U) return 1;
    if (std::abs(sum - 0.3) > 1e-12) return 2;
    std::cout << "PASS\n";
}
