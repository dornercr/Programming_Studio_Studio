#include <cassert>
#include <iostream>
#include <stdexcept>

unsigned misses(unsigned size, unsigned elements_per_line, bool rows_first) {
    if(size == 0 || size > 64 || elements_per_line == 0 || elements_per_line > 64)
        throw std::invalid_argument("model dimensions");
    int resident = -1; unsigned count = 0;
    for(unsigned outer=0; outer<size; ++outer)
        for(unsigned inner=0; inner<size; ++inner) {
            const unsigned index = rows_first ? outer*size+inner : inner*size+outer;
            const int block = static_cast<int>(index/elements_per_line);
            if(block != resident) { ++count; resident = block; }
        }
    return count;
}

int main() {
    assert(misses(4,4,true) == 4 && misses(4,4,false) == 16);
    assert(misses(1,1,true) == 1 && misses(1,1,false) == 1);
    int rejected = 0;
    try { misses(0,4,true); } catch(const std::invalid_argument&) { ++rejected; }
    try { misses(4,0,true); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "rows=" << misses(4,4,true) << " columns=" << misses(4,4,false) << '\n';
}
