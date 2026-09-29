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
