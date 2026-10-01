// LAB: Make subregister width an explicit input
// Model writes of 16, 32, or 64 bits to a 64-bit x86 register. A 32-bit write clears the high half; a 16-bit write preserves the other bits. Reject unsupported widths.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstdint>
#include <iostream>

int main() {
    const std::uint64_t old = 0xFFFFFFFF00000000ULL;
    const std::uint32_t eax = 7;
    const std::uint64_t after_eax = eax;
    const std::uint64_t after_ax = (old & ~0xFFFFULL) | 7ULL;
    std::cout << std::hex << after_eax << ' ' << after_ax << '\n';
    assert(after_eax == 7 && after_ax == 0xFFFFFFFF00000007ULL);
}
