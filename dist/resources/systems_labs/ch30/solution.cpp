#include <cassert>
#include <iostream>
#include <optional>
#include <vector>
#include <stdexcept>

unsigned hits(const std::vector<unsigned>& addresses, unsigned line_size, unsigned slots) {
    if(line_size == 0 || slots == 0) throw std::invalid_argument("cache geometry");
    std::vector<std::optional<unsigned>> resident(slots);
    unsigned count = 0;
    for(unsigned address : addresses) {
        const unsigned block = address/line_size, slot = block%slots;
        if(resident[slot] && *resident[slot] == block) ++count;
        resident[slot] = block;
    }
    return count;
}

int main() {
    assert(hits({0,16,0,64,0,16},16,4) == 2);
    assert(hits({},16,4) == 0 && hits({0,0,0},16,4) == 2);
    bool rejected = false;
    try { hits({0},0,4); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "trace-hits=" << hits({0,16,0,64,0,16},16,4)
              << " repeat-hits=" << hits({0,0,0},16,4) << '\n';
}
