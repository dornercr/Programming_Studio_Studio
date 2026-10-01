#include <cassert>
#include <cstddef>
#include <iostream>
#include <optional>

std::optional<std::size_t> reserve_bytes(std::size_t& used, std::size_t capacity,
                                        std::size_t request) {
    if(used > capacity) return std::nullopt;
    const auto padding = (8 - used%8)%8;
    if(padding > capacity-used) return std::nullopt;
    const auto start = used+padding;
    if(request > capacity-start) return std::nullopt;
    used = start+request;
    return start;
}

int main() {
    std::size_t used = 0;
    const auto first = reserve_bytes(used,32,5);
    const auto second = reserve_bytes(used,32,9);
    assert(first == 0 && second == 8 && used == 17);
    const auto before = used;
    assert(!reserve_bytes(used,32,20) && used == before);
    std::cout << "starts=" << *first << ',' << *second << " used=" << used << '\n';
}
