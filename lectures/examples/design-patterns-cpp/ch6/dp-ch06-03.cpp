#include <iostream>

double bad_grams(int raw) {
    return static_cast<double>(raw / 1000); // The fraction is already lost.
}
int main() {
    const int measured = 1999;
    const int failed = -1;
    std::cout << "lost=" << bad_grams(measured) << '\n';
    std::cout << "preserved=" << measured / 1000.0 << '\n';
    // This deliberately wrong rule confuses failure with weight.
    std::cout << std::boolalpha << "failure accepted="
              << (failed / 1000.0 <= 2.0) << '\n';
}
