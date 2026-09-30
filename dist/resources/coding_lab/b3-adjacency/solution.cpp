#include <iostream>
#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>

std::vector<std::vector<std::size_t>> adjacency(std::size_t n,const std::vector<std::pair<std::size_t,std::size_t>>& edges){
    if(n>100) throw std::invalid_argument("graph");
    std::vector<std::vector<std::size_t>> g(n);
    for(auto [a,b]:edges){
        if(a>=n||b>=n) throw std::invalid_argument("edge");
        g[a].push_back(b);
    }
    return g;
}
