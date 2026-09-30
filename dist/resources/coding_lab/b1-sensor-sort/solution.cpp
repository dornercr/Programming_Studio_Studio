#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

std::vector<int> descending(const std::vector<int>& readings) {
    auto copy=readings;
    std::sort(copy.begin(),copy.end(),std::greater<int>{});
    return copy;
}
