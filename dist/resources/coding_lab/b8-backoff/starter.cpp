#include <iostream>
#include <cstddef>
#include <algorithm>
#include <limits>

std::size_t retryDelay(unsigned failures, std::size_t base, std::size_t cap) {
    // TODO: grow the delay, but never wrap or exceed cap.
    return base;
}
