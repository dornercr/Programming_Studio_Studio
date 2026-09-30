// Original book listing B01-L0133
#ifndef HARBOR_CHECKED_OFFSET_HPP
#define HARBOR_CHECKED_OFFSET_HPP
#include <vector>

namespace harbor {
// Returns false without changing any element when an addition would overflow.
// Precondition: no concurrent mutation of values during either pass.
bool apply_offset_checked(std::vector<int>& values, int offset);
}
#endif

// Original book listing B01-L0134
#include <limits>

namespace harbor {
bool apply_offset_checked(std::vector<int>& values, int offset) {
    const int low = std::numeric_limits<int>::min();
    const int high = std::numeric_limits<int>::max();
    for (const int value : values) {
        if (offset > 0 && value > high - offset) return false;
        if (offset < 0 && value < low - offset) return false;
    }
    // Validation of the entire batch precedes all mutation.
    for (int& value : values) value += offset;
    return true;
}
}

// Original book listing B01-L0135
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::vector<int> ordinary{1, 2, 3};
    if (!harbor::apply_offset_checked(ordinary, 2)) return 1;
    if (ordinary != std::vector<int>{3, 4, 5}) return 2;

    const int maximum = std::numeric_limits<int>::max();
    const int minimum = std::numeric_limits<int>::min();
    std::vector<int> overflow{5, maximum};
    const auto original = overflow;
    if (harbor::apply_offset_checked(overflow, 1)) return 3;
    if (overflow != original) return 4;

    std::vector<int> underflow{minimum, 8};
    const auto before_underflow = underflow;
    if (harbor::apply_offset_checked(underflow, -1)) return 5;
    if (underflow != before_underflow) return 6;

    std::vector<int> edge{0};
    if (!harbor::apply_offset_checked(edge, minimum)) return 7;
    if (edge.front() != minimum) return 8;
    if (!harbor::apply_offset_checked(edge, maximum)) return 9;
    if (edge.front() != -1) return 10;

    std::vector<int> empty;
    if (!harbor::apply_offset_checked(empty, maximum)) return 11;
    std::cout << "checked offset: valid, overflow, underflow, "
                 "extreme offset, and empty cases PASS\n";
}
