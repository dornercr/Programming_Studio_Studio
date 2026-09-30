#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>

bool allowed(std::size_t offset, std::size_t length, std::size_t extent) {
    return offset <= extent && length <= extent-offset;
}

int main() {
    assert(allowed(4,8,16));
    assert(allowed(16,0,16));
    assert(!allowed(17,0,16) && !allowed(15,2,16));
    assert(!allowed(1,std::numeric_limits<std::size_t>::max(),16));
    std::cout << "valid=accepted outside=rejected\n";
}
