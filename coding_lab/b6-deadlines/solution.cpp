#include <iostream>
#include <vector>
#include <cstddef>
#include <stdexcept>

std::size_t deadlineMisses(const std::vector<int>& completed, const std::vector<int>& deadlines) {
    if(completed.size()!=deadlines.size()) throw std::invalid_argument("size mismatch");
    std::size_t misses=0;
    for(std::size_t i=0;i<completed.size();++i) if(completed[i]>deadlines[i]) ++misses;
    return misses;
}
