// LAB: Check a copy before touching the destination
// Copy a source string into an existing character array only if the characters and trailing null fit. Return false without writing anything when the capacity is too small.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<int> values{8, 9, 10};
    try {
        std::cout << values.at(3) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "out of range\n";
    }
    assert(values.size() == 3);
}
