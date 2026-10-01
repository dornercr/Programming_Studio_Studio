#include <iostream>
int main() {
    const int negative = -7;
    const int positive = 7;
    std::cout << "negative division=" << negative / 2
              << " shift=" << (negative >> 1) << '\n';
    std::cout << "positive division=" << positive / 2
              << " shift=" << (positive >> 1) << '\n';
}
