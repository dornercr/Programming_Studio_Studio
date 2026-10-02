#include <iostream>
int main() {
    double subtotal = 120.0;
    double total{};
    if (subtotal >= 100.0) total = subtotal * 0.90;
    else total = subtotal;
    std::cout << total << '\n';
}
