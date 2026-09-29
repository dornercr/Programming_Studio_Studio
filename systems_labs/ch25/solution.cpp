#include <cassert>
#include <iostream>
#include <map>
#include <optional>

std::optional<unsigned> lookup(unsigned page, const std::map<unsigned,unsigned>& table,
    std::map<unsigned,unsigned>& tlb, unsigned& walks) {
    const auto hit = tlb.find(page);
    if(hit != tlb.end()) return hit->second;
    ++walks;
    const auto entry = table.find(page);
    if(entry == table.end()) return std::nullopt;
    tlb.emplace(page,entry->second);
    return entry->second;
}

int main() {
    const std::map<unsigned,unsigned> table{{1,7}};
    std::map<unsigned,unsigned> tlb; unsigned walks = 0;
    assert(lookup(1,table,tlb,walks) == 7 && lookup(1,table,tlb,walks) == 7 && walks == 1);
    assert(!lookup(2,table,tlb,walks) && tlb.count(2) == 0 && walks == 2);
    tlb.clear();
    assert(lookup(1,table,tlb,walks) == 7 && walks == 3);
    std::cout << "walks=" << walks << " cached=" << tlb.size() << '\n';
}
