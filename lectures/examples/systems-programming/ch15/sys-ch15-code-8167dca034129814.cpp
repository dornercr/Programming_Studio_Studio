// LAB: Guard signed division boundaries
// Return quotient and remainder for int inputs. Reject division by zero and the minimum int divided by -1, whose quotient cannot fit in int.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
