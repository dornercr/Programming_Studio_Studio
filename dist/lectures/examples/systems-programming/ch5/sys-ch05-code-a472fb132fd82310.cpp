#include <cmath>
#include <iostream>
#include <limits>
bool accepted_measurement(double value) {
    return std::isfinite(value) && value >= 0.0;
}
int main() {
    static_assert(std::numeric_limits<double>::has_quiet_NaN);
    const double missing = std::numeric_limits<double>::quiet_NaN();
    std::cout << std::boolalpha;
    std::cout << "self equality=" << (missing == missing) << '\n';
    std::cout << "is NaN=" << std::isnan(missing) << '\n';
    std::cout << "measurement accepted=" << accepted_measurement(missing) << '\n';
    std::cout << "zero accepted=" << accepted_measurement(0.0) << '\n';
}
