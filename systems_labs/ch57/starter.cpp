// LAB: Validate a complete bounded frame extent
// Return whether a header and claimed payload fit inside a received extent, with a separate maximum payload of eight. Check the header before subtracting.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const std::size_t buffer_size = 10;
    const std::size_t header_size = 8;
    const std::size_t claimed_payload = 5;
    const bool valid = header_size <= buffer_size
        && claimed_payload <= buffer_size - header_size;
    std::cout << (valid ? "accept" : "reject") << '\n';
    assert(!valid);
}
