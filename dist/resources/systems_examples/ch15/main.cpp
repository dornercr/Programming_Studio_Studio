#include <cassert>
#include <iostream>

int main() {
    const int value = -17;
    const int divisor = 5;
    const int quotient = value / divisor;
    const int remainder = value % divisor;
    std::cout << quotient << ' ' << remainder << '\n';
    assert(quotient * divisor + remainder == value);
}
