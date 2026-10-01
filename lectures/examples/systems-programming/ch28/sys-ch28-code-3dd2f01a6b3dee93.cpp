// LAB: Generalize the traversal trace
// Count misses for a square row-major array in a one-line cache. The dimension and elements per line are inputs from 1 through 64; reject zero. Compare both traversal orders.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
