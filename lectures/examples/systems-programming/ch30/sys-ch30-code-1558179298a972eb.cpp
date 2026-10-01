#include <algorithm>
#include <array>
#include <iostream>
#include <list>
#include <optional>
int main() {
    std::array<std::optional<unsigned>,2> direct;
    std::list<unsigned> lru; // Most recently used is at the front.
    unsigned direct_hits = 0, reference_hits = 0;
    for (unsigned block : {0u,2u,0u,2u}) {
        auto& slot = direct[block%2];
        if (slot && *slot == block) ++direct_hits;
        slot = block;
        const auto found = std::find(lru.begin(),lru.end(),block);
        if (found != lru.end()) { ++reference_hits; lru.erase(found); }
        else if (lru.size() == 2) lru.pop_back();
        lru.push_front(block);
    }
    std::cout << "direct-hits=" << direct_hits
              << " fully-associative-hits=" << reference_hits << '\n';
}
