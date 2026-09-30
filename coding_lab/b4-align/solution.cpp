#include <iostream>
#include <cstdint>
#include <limits>
#include <stdexcept>

std::uint64_t alignedSize(std::uint64_t bytes, std::uint64_t alignment) {
    if(alignment==0) throw std::invalid_argument("zero alignment");
    auto remainder=bytes%alignment;
    if(remainder==0) return bytes;
    auto extra=alignment-remainder;
    if(bytes>std::numeric_limits<std::uint64_t>::max()-extra) throw std::overflow_error("size overflow");
    return bytes+extra;
}
