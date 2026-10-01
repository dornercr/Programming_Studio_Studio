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
