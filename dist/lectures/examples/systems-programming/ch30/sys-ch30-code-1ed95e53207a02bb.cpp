// LAB: Make cold-cache behavior a reusable function
// Simulate reads for a configurable direct-mapped cache. Each call begins cold; use explicit empty slots and return the hit count. Reject zero line size or zero slot count.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <array>
#include <cassert>
#include <iostream>

int main() {
    std::array<int, 4> tags{-1, -1, -1, -1};
    int hits = 0;
    for (int address : {0, 16, 0, 64, 0, 16}) {
        const int block = address / 16;
        const int slot = block % 4;
        const bool hit = tags[slot] == block;
        hits += hit;
        tags[slot] = block;
        std::cout << (hit ? 'H' : 'M') << ' ';
    }
    std::cout << "hits=" << hits << '\n';
    assert(hits == 2);
}
