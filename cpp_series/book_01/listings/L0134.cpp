#include "checked_offset.hpp"
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
