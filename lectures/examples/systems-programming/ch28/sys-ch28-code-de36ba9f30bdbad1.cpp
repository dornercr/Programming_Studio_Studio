#include <cassert>
#include <iostream>

int main() {
    const auto misses = [](bool by_row) {
        int resident = -1, count = 0;
        for (int outer = 0; outer < 4; ++outer)
            for (int inner = 0; inner < 4; ++inner) {
                const int index = by_row ? outer * 4 + inner : inner * 4 + outer;
                const int block = index / 4;
                if (block != resident) { ++count; resident = block; }
            }
        return count;
    };
    std::cout << misses(true) << ' ' << misses(false) << '\n';
    assert(misses(true) == 4 && misses(false) == 16);
}
