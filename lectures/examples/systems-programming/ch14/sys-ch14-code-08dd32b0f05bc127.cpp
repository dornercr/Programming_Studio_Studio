#include <cassert>
#include <cstddef>
#include <iostream>
#include <optional>

std::optional<std::size_t> effective(std::size_t base, std::size_t index,
    std::size_t scale, std::size_t displacement, std::size_t limit) {
    if(base > limit) return std::nullopt;
    if(scale != 0 && index > (limit-base)/scale) return std::nullopt;
    const auto partial = base+index*scale;
    if(displacement > limit-partial) return std::nullopt;
    return partial+displacement;
}

int main() {
    const auto result = effective(1000,3,4,8,2000);
    assert(result == 1020);
    assert(!effective(1000,300,4,0,2000));
    assert(!effective(1999,0,4,2,2000));
    assert(effective(10,0,4,2,20) == 12);
    std::cout << "address=" << *result << " invalid=rejected\n";
}
