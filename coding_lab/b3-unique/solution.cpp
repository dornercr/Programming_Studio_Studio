#include <iostream>
#include <vector>
#include <unordered_set>

std::vector<int> stableUnique(const std::vector<int>& values) {
    std::unordered_set<int> seen;
    std::vector<int> result;
    for(int x:values) if(seen.insert(x).second) result.push_back(x);
    return result;
}
