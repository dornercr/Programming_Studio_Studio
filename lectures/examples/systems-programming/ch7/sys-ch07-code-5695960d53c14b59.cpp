#include <iostream>
int main() {
    int value = 4;
    int* first = &value;
    int* second = &value;
    *first = 9;
    std::cout << "through second=" << *second << '\n';
    const int snapshot = *second;
    *first = 12;
    std::cout << "copied snapshot=" << snapshot << '\n';
    std::cout << "current value=" << *second << '\n';
}
