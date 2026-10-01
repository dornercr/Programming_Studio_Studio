#include <iomanip>
#include <iostream>

int main() {
    const double fahrenheit = 100.0;
    const double limit_celsius = 80.0;
    // Deliberate baseline defect: compare unlike units.
    std::cout << "raw comparison="
              << (fahrenheit >= limit_celsius ? "ALERT" : "ok") << '\n';
    const double celsius = (fahrenheit - 32.0) * 5.0 / 9.0;
    std::cout << std::fixed << std::setprecision(1)
              << "normalized=" << celsius << " C\n";
    std::cout << "normalized comparison="
              << (celsius >= limit_celsius ? "ALERT" : "ok") << '\n';
}
