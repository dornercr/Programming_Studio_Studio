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
