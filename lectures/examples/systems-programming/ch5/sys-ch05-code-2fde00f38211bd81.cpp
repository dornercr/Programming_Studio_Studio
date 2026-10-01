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
