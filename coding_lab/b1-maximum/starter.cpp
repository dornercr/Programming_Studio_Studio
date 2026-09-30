#include <iostream>
#include <vector>
#include <stdexcept>

int maximum(const std::vector<int>& values) {
    if(values.empty()) throw std::invalid_argument("empty input");
    int best=0; // BUG: what if every value is negative?
    for(int x : values) if(x>best) best=x;
    return best;
}
