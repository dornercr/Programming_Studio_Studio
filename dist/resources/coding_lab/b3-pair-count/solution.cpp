#include <iostream>
#include <cstdint>
#include <stdexcept>

std::uint64_t pairCount(std::uint64_t n){if(n>1000000)throw std::invalid_argument("too many items");if(n<2)return 0;return n*(n-1)/2;}
