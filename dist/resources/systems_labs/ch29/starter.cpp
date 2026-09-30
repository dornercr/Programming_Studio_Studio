// LAB: Decompose a cache address for variable geometry
// Return block, set, and tag for nonzero line size and set count. Accept byte addresses as unsigned integers; reject zero geometry.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    for (unsigned address : {0u, 16u, 64u}) {
        const unsigned block = address / 16;
        const unsigned set = block % 4;
        const unsigned tag = block / 4;
        std::cout << address << ": set=" << set << " tag=" << tag << '\n';
    }
}
