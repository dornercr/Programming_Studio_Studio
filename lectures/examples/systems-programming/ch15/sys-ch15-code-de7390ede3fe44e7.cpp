#include <cassert>
#include <iostream>
#include <limits>
#include <optional>
#include <utility>

std::optional<std::pair<int,int>> divide(int value, int divisor) {
    if(divisor == 0) return std::nullopt;
    if(value == std::numeric_limits<int>::min() && divisor == -1) return std::nullopt;
    return std::pair<int,int>{value/divisor,value%divisor};
}

int main() {
    const auto first = divide(-17,5);
    const auto second = divide(17,-5);
    assert(first && first->first == -3 && first->second == -2);
    assert(second && second->first == -3 && second->second == 2);
    assert(!divide(1,0));
    assert(!divide(std::numeric_limits<int>::min(),-1));
    std::cout << first->first << ' ' << first->second << " invalid=rejected\n";
}
