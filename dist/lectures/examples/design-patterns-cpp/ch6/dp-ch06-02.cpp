#include <iostream>
#include <stdexcept>

double grams_from(int milligrams) {
    if (milligrams < 0) throw std::runtime_error("device failed");
    return milligrams / 1000.0; // A double divisor keeps the fraction.
}
int main() {
    std::cout << "grams=" << grams_from(1250) << '\n';
    try { std::cout << grams_from(-1) << '\n'; }
    catch (const std::runtime_error&) { std::cout << "reading rejected\n"; }
}
