#include <iostream>
#include <cstddef>
#include <stdexcept>

std::size_t advance(std::size_t completed, std::size_t reported, std::size_t requested) {
    if(completed>requested || reported>requested-completed) throw std::invalid_argument("invalid progress");
    return completed+reported;
}
