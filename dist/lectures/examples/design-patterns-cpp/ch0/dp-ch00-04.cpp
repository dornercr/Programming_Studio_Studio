#include <iostream>

// Precondition: both readings are in [0, 1000].
int mean(int left, int right) {
    return (left + right) / 2; // Integer division drops fractions.
}

int main() {
    std::cout << "mean=" << mean(2, 8) << '\n';
}
