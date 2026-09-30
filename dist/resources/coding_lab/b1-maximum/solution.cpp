#include <iostream>
#include <vector>
#include <stdexcept>

int maximum(const std::vector<int>& values) {
    if(values.empty()) throw std::invalid_argument("empty input");
    int best=values.front();
    for(int x : values) if(x>best) best=x;
    return best;
}
