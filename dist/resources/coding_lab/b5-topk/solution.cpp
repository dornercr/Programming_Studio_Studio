#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstddef>

std::vector<int> topK(const std::vector<int>& values, std::size_t k) {
    auto result=values;
    k=std::min(k,result.size());
    std::partial_sort(result.begin(),result.begin()+k,result.end(),std::greater<int>{});
    result.resize(k);
    return result;
}
