#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <optional>

std::optional<std::size_t> checked_bytes(std::size_t count, std::size_t width,
                                        std::size_t limit) {
    if(width != 0 && count > limit / width) return std::nullopt;
    return count * width;
}

int main() {
    const auto accepted = checked_bytes(8,12,100);
    assert(accepted && *accepted == 96);
    assert(!checked_bytes(9,12,100));
    assert(checked_bytes(0,12,100) == 0 && checked_bytes(9,0,100) == 0);
    const auto maximum = std::numeric_limits<std::size_t>::max();
    assert(!checked_bytes(maximum,2,maximum));
    std::cout << "bytes=" << *accepted << " oversized=rejected\n";
}
