// LAB: Translate through a checked page map
// Translate byte addresses using 4096-byte pages and a virtual-page-to-frame map. Reject an unmapped virtual page and guard the frame multiplication.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const unsigned page_size = 4096;
    const unsigned address = 0x1234;
    const unsigned page = address / page_size;
    const unsigned offset = address % page_size;
    const unsigned frame = 7;
    const unsigned physical = frame * page_size + offset;
    std::cout << "page=" << page << " offset=" << offset
              << " physical=" << physical << '\n';
    assert(page == 1 && offset == 564 && physical == 29236);
}
