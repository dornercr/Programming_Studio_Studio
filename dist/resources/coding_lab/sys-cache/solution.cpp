#include <iostream>
#include <cstdint>
#include <stdexcept>

std::uint64_t cacheSet(std::uint64_t address, std::uint64_t blockSize, std::uint64_t sets) {
    if(blockSize==0||sets==0) throw std::invalid_argument("zero dimension");
    return (address/blockSize)%sets;
}
