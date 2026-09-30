#include <iostream>
#include <cstddef>
#include <algorithm>
#include <limits>

std::size_t retryDelay(unsigned failures, std::size_t base, std::size_t cap) {
    std::size_t delay=std::min(base,cap);
    if(delay==0) return 0;
    while(failures-- && delay<cap){
        if(delay>cap/2) return cap;
        delay*=2;
    }
    return delay;
}
