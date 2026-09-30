// LAB: Represent a call contract in C++
// Write a helper that doubles a small integer and a caller that preserves a separate value across that call. Test positive, zero, and negative inputs.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const auto helper = [](int argument) { return argument * 2; };
    const int saved = 10;
    const int returned = helper(3);
    std::cout << saved + returned << '\n';
    assert(saved + returned == 16);
}
