#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <map>
#include <optional>

std::optional<std::size_t> translate(std::size_t address,
    const std::map<std::size_t,std::size_t>& pages) {
    const std::size_t size = 4096;
    const auto entry = pages.find(address/size);
    if(entry == pages.end()) return std::nullopt;
    const auto offset = address%size;
    const auto maximum = std::numeric_limits<std::size_t>::max();
    if(entry->second > (maximum-offset)/size) return std::nullopt;
    return entry->second*size+offset;
}

int main() {
    std::map<std::size_t,std::size_t> pages{{1,7}};
    assert(translate(0x1234,pages) == 29236 && translate(0x1235,pages) == 29237);
    assert(!translate(0x2000,pages));
    pages[2] = std::numeric_limits<std::size_t>::max();
    assert(!translate(0x2000,pages));
    std::cout << "physical=" << *translate(0x1234,pages) << " unmapped=rejected\n";
}
