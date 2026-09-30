#include <iostream>
#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>

long long matrixSum(const std::vector<int>& data,std::size_t width){(void)width;long long sum=0;for(int x:data)sum+=x;return sum;}
