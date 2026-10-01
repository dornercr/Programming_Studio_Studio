// LAB: Count TLB walks separately from data hits
// Model a translation cache over a fixed page table. The first lookup of a mapped page performs a walk; the second reuses the translation. An unmapped page must not enter the TLB.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    bool tlb_has_mapping = false;
    const bool page_present = true;
    const bool data_cached = true;
    int walks = 0;
    if (!tlb_has_mapping) {
        ++walks;
        assert(page_present);
        tlb_has_mapping = true;
    }
    std::cout << "walks=" << walks << " data="
              << (data_cached ? "hit" : "miss") << '\n';
}
