#include <iostream>
#include <vector>
#include <functional>

int applyAll(int start, const std::vector<std::function<int(int)>>& operations) {
    for(const auto& operation : operations) start=operation(start);
    return start;
}
