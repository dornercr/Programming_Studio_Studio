#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

std::pair<int,int> accepted(const std::vector<int>& readings) {
    int count = 0, sum = 0;
    for (int value : readings) {
        if (value == -999) break; // Sentinel is not data.
        if (value < 0) continue;
        sum += value;
        ++count;
        if (count == 3) break; // Bound accepted items, not attempts.
    }
    return {count, sum};
}

int main() {
    std::vector<int> v;
    int n;
    while(std::cin>>n)v.push_back(n);
    auto p=accepted(v);
    std::cout<<p.first<<" "<<p.second<<"\n";
    }
