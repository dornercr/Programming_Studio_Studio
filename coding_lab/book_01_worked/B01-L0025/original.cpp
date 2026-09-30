#include <iomanip>
#include <iostream>
int main() {
    double price = 12.5;
    int quantity = 3;
    std::cout << "quantity=" << quantity
              << " total=$" << std::fixed << std::setprecision(2)
              << price * quantity << '\n';
}
