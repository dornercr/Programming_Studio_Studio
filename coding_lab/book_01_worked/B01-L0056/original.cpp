#include <iostream>
void increment_copy(int x) { ++x; }
void increment_original(int& x) { ++x; }
int main() {
    int n = 10;
    increment_copy(n);
    std::cout << n << ' ';
    increment_original(n);
    std::cout << n << '\n';
}
