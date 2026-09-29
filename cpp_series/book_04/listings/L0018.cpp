#include <iostream>

void inner(int value) {
    int local = value + 1;
    std::cout << "inner local @ " << &local << "\n";
}

void outer() {
    int local = 10;
    std::cout << "outer local @ " << &local << "\n";
    inner(local);
}

int main() { outer(); }
