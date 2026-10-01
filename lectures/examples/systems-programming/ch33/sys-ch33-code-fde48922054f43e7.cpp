// LAB: Read a bounded record slice
// Return a substring only when offset and length fit within the file model. An empty slice at end is valid; an offset past end is not.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
    const std::string file = "ABCDE";
    const std::size_t offset = 2;
    const std::size_t count = 3;
    assert(offset <= file.size() && count <= file.size() - offset);
    std::cout << file.substr(offset, count) << '\n';
}
