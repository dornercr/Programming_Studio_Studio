#include <iostream>
#include <vector>
#include <cstddef>

std::size_t lowerBound(const std::vector<int>& values, int target) {
    std::size_t low=0,high=values.size();
    while(low<high){
        auto mid=low+(high-low)/2;
        if(values[mid]<target) low=mid+1;
        else high=mid;
    }
    return low;
}
