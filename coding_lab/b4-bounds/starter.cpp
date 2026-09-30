#include <iostream>
#include <cstddef>
#include <limits>

bool fits(std::size_t offset, std::size_t length, std::size_t size) {
    return offset + length <= size; // BUG: addition can wrap.
}
