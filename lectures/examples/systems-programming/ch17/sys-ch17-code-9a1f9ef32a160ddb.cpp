#include <iostream>
unsigned total(unsigned n) {
    std::cout << "enter " << n << '\n';
    if (n == 0) return 0;
    const unsigned below = total(n - 1);
    const unsigned result = n + below;
    std::cout << "return " << result << '\n';
    return result;
}
int main() {
    const auto result = total(3); // Bounded demonstration depth.
    std::cout << "total=" << result << '\n';
}
