// LAB: Validate a packed-field setter
// Create a setter for bits 4–7 of an unsigned word. Reject values above 15 and preserve all bits outside that field.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const unsigned old_value = 0xA5u;
    const unsigned field = 3u;
    const unsigned mask = 0xFu << 4;
    const unsigned result = (old_value & ~mask) | (field << 4);
    std::cout << std::hex << result << '\n';
    assert(result == 0x35u);
}
