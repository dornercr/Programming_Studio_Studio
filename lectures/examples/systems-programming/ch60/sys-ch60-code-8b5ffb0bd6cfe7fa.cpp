#include <iostream>
int scale = 7;
int evaluate(int value) { return value * scale; }
int main() {
    std::cout << "first=" << evaluate(3) << '\n';
    scale = 9;
    std::cout << "second=" << evaluate(3) << '\n';
}
