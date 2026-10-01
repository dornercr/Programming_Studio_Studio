#include <limits>
#include <cassert>
#include <iostream>
#include <stdexcept>

static_assert(std::numeric_limits<unsigned>::max() >= 499500u, "32-bit unsigned required");
unsigned sum_before(unsigned limit) {
    if(limit > 1000) throw std::invalid_argument("limit");
    unsigned total = 0;
    for(unsigned i = 0; i < limit; ++i) total += i;
    return total;
}

int main() {
    assert(sum_before(0) == 0 && sum_before(1) == 0);
    assert(sum_before(4) == 6 && sum_before(1000) == 499500);
    bool rejected = false;
    try { sum_before(1001); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "empty=" << sum_before(0) << " four=" << sum_before(4) << '\n';
}
