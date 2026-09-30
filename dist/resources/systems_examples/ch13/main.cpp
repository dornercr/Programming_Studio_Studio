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
