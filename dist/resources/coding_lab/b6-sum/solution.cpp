#include <iostream>
#include <vector>
#include <future>
#include <cstddef>

long long parallelSum(const std::vector<int>& values) {
    auto sum=[&](std::size_t begin,std::size_t end){
        long long total=0;
        for(auto i=begin;i<end;++i) total+=values[i];
        return total;
    };
    auto mid=values.size()/2;
    auto first=std::async(std::launch::async,sum,0,mid);
    auto second=std::async(std::launch::async,sum,mid,values.size());
    return first.get()+second.get();
}
