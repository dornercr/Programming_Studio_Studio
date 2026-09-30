#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
    const std::string file = "ABCDE";
    const std::size_t offset = 2;
    const std::size_t count = 3;
    assert(offset <= file.size() && count <= file.size() - offset);
    std::cout << file.substr(offset, count) << '\n';
}
