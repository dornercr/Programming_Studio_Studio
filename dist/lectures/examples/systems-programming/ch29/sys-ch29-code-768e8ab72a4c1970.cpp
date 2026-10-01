#include <cassert>
#include <iostream>
#include <stdexcept>

struct Parts { unsigned block; unsigned set; unsigned tag; };
Parts decompose(unsigned address, unsigned line_size, unsigned sets) {
    if(line_size == 0 || sets == 0) throw std::invalid_argument("cache geometry");
    const unsigned block = address/line_size;
    return {block,block%sets,block/sets};
}

int main() {
    const auto first = decompose(64,16,4), last = decompose(79,16,4);
    assert(first.block == 4 && first.set == 0 && first.tag == 1);
    assert(first.block == last.block && first.set == last.set && first.tag == last.tag);
    int rejected = 0;
    try { decompose(0,0,4); } catch(const std::invalid_argument&) { ++rejected; }
    try { decompose(0,16,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "block=" << first.block << " set=" << first.set << " tag=" << first.tag << '\n';
}
