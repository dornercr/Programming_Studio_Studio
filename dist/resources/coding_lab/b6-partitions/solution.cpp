#include <iostream>
#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>

std::vector<std::pair<std::size_t,std::size_t>> partition(std::size_t n, std::size_t workers) {
    if(workers==0) throw std::invalid_argument("zero workers");
    std::vector<std::pair<std::size_t,std::size_t>> ranges;
    std::size_t begin=0;
    for(std::size_t i=0;i<workers;++i){
        auto length=n/workers+(i<n%workers?1:0);
        ranges.push_back({begin,begin+length});
        begin+=length;
    }
    return ranges;
}
