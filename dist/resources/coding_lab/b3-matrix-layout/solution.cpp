#include <iostream>
#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>

long long matrixSum(const std::vector<int>& data,std::size_t width){
    if(width==0){if(!data.empty())throw std::invalid_argument("shape");return 0;}
    if(data.size()/width!=width||data.size()%width!=0) throw std::invalid_argument("shape");
    long long sum=0;
    for(std::size_t row=0;row<width;++row)
        for(std::size_t col=0;col<width;++col) sum+=data[row*width+col];
    return sum;
}
