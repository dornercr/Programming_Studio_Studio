#include <array>
#include <iostream>
int main() {
    constexpr unsigned page_size = 8;
    const std::array<unsigned,2> frames{3,9};
    // Integer translation model: no physical memory is accessed.
    for (unsigned address : {7u,8u,9u}) {
        const unsigned page = address/page_size;
        const unsigned offset = address%page_size;
        std::cout << address << ": page=" << page << " offset=" << offset
                  << " physical=" << frames.at(page)*page_size+offset << '\n';
    }
}
