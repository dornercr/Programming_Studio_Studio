#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>
#include <future>

long long parallel_sum(const std::vector<int>& values) {
    const auto middle = values.size()/2;
    const auto part = [&](std::size_t begin, std::size_t end) {
        return std::accumulate(values.begin()+begin,values.begin()+end,0LL);
    };
    auto first = std::async(std::launch::async,part,0,middle);
    auto second = std::async(std::launch::async,part,middle,values.size());
    return first.get()+second.get();
}

int main() {
    assert(parallel_sum({1,2,3,4,5}) == 15);
    assert(parallel_sum({}) == 0 && parallel_sum({7}) == 7);
    std::cout << "odd=" << parallel_sum({1,2,3,4,5}) << " empty=" << parallel_sum({}) << '\n';
}
