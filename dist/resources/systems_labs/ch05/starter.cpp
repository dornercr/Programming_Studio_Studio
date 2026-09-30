// LAB: Compare nearby floating-point results
// Implement a relative-and-absolute closeness test for finite doubles. Make the tolerances explicit and reject negative tolerances. Exact equality handles equal infinities before the finite-only calculation.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <limits>

int main() {
    static_assert(std::numeric_limits<double>::is_iec559);
    static_assert(std::numeric_limits<double>::digits == 53);
    const double large = 9007199254740992.0;
    const double sum = large + 1.0;
    std::cout << std::boolalpha << (sum == large) << '\n';
    assert(sum == large);
}
