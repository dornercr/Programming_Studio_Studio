#include <iostream>
#include <limits>
#include <stdexcept>

int narrow(long long value){
    if(value<std::numeric_limits<int>::min()||value>std::numeric_limits<int>::max())throw std::out_of_range("outside int range");
    return static_cast<int>(value);
}
