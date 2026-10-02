#ifndef HARBOR_CHECKED_OFFSET_HPP
#define HARBOR_CHECKED_OFFSET_HPP
#include <vector>

namespace harbor {
// Returns false without changing any element when an addition would overflow.
// Precondition: no concurrent mutation of values during either pass.
bool apply_offset_checked(std::vector<int>& values, int offset);
}
#endif
