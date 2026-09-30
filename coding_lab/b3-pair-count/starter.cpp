#include <iostream>
#include <cstdint>
#include <stdexcept>

std::uint64_t pairCount(std::uint64_t n){if(n>1000000)throw std::invalid_argument("too many items");return n*n;}
