#include <array>
#include <future>
#include <numeric>
#include <iostream>

int main() {
    std::array<int,6> input{1,2,3,4,5,6}, squared{};
    auto transform=[&](std::size_t begin,std::size_t end) {
        for(auto i=begin;i<end;++i) squared[i]=input[i]*input[i];
    };
    auto first=std::async(std::launch::async,transform,0,3);
    auto second=std::async(std::launch::async,transform,3,6);
    first.get(); second.get();
    std::cout << "squares=";
    for(auto x:squared) std::cout << x << ' ';
    std::cout << "sum=" << std::accumulate(squared.begin(),squared.end(),0) << '\n';
}
