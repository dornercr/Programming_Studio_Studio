#include <array>
#include <iostream>
#include <optional>
int main() {
    const std::array<unsigned,4> pages{4,4,9,4};
    const std::array<bool,4> data_cached{true,false,true,true};
    std::optional<unsigned> entry;
    unsigned walks = 0;
    for (std::size_t i=0; i<pages.size(); ++i) {
        const bool hit = entry && *entry == pages[i];
        if (!hit) { ++walks; entry = pages[i]; }
        std::cout << "page=" << pages[i] << " tlb=" << (hit ? "hit" : "miss")
                  << " data=" << (data_cached[i] ? "hit" : "miss") << '\n';
    }
    std::cout << "walks=" << walks << '\n';
}
