#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>

bool valid_frame(std::size_t received, std::size_t header, std::size_t payload) {
    return header <= received && payload <= 8 && payload <= received-header;
}

int main() {
    assert(valid_frame(5,2,3));
    assert(!valid_frame(1,2,0) && !valid_frame(4,2,3));
    assert(!valid_frame(100,2,9));
    assert(!valid_frame(10,2,std::numeric_limits<std::size_t>::max()));
    std::cout << "complete=valid truncated=rejected oversized=rejected\n";
}
