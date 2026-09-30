#include <cassert>
#include <iostream>
#include <stdexcept>

unsigned set_field(unsigned word, unsigned value) {
    if(value > 15u) throw std::invalid_argument("four-bit field");
    const unsigned mask = 0xFu << 4;
    return (word & ~mask) | (value << 4);
}

int main() {
    assert(set_field(0xA5u,3u) == 0x35u);
    assert(set_field(0xA5u,0u) == 0x05u);
    assert(set_field(0xA5u,15u) == 0xF5u);
    assert((set_field(0x1234u,9u) & ~(0xFu << 4)) == (0x1234u & ~(0xFu << 4)));
    bool rejected = false;
    try { set_field(0,16); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << std::hex << set_field(0xA5u,3u) << " rejected=" << rejected << '\n';
}
