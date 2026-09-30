#include <iostream>
#include <limits>
int main() {
    std::cout << "int max=" << std::numeric_limits<int>::max() << '\n';
    int whole = 7;
    int divisor = 2;
    double exact = static_cast<double>(whole) / divisor;
    std::cout << "int division=" << whole / divisor << " floating=" << exact << '\n';
}
