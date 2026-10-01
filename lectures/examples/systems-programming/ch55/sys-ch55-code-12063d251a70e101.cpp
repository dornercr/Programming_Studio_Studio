#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stdexcept>

std::vector<long long> prefix(const std::vector<int>& values) {
    if(values.size() > 1000000) throw std::invalid_argument("count");
    std::vector<long long> result(values.size()+1,0);
    for(std::size_t i=0;i<values.size();++i) {
        if(values[i] < -1000 || values[i] > 1000) throw std::invalid_argument("value");
        result[i+1] = result[i]+values[i];
    }
    return result;
}
long long query(const std::vector<long long>& sums, std::size_t left, std::size_t right) {
    if(sums.empty() || left > right || right >= sums.size()) throw std::out_of_range("range");
    return sums[right]-sums[left];
}

int main() {
    const auto sums = prefix({2,4,6,8});
    assert(query(sums,1,4) == 18 && query(sums,2,2) == 0 && query(sums,0,4) == 20);
    int rejected = 0;
    try { query(sums,3,2); } catch(const std::out_of_range&) { ++rejected; }
    try { query(sums,0,5); } catch(const std::out_of_range&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "range=" << query(sums,1,4) << " empty=" << query(sums,2,2) << '\n';
}
