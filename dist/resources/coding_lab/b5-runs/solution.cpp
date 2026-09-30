#include <iostream>
#include <vector>
#include <cstddef>

std::size_t countRuns(const std::vector<int>& values) {
    if(values.empty()) return 0;
    std::size_t runs=1;
    for(std::size_t i=1;i<values.size();++i) if(values[i]!=values[i-1]) ++runs;
    return runs;
}
