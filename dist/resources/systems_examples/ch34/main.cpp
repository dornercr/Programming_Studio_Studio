#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

int main() {
    const std::string input = "ABCDE";
    std::string output;
    std::size_t position = 0;
    int calls = 0;
    while (position < input.size()) {
        const auto count = std::min<std::size_t>(2, input.size() - position);
        output.append(input, position, count);
        position += count;
        ++calls;
    }
    std::cout << output << " calls=" << calls << '\n';
    assert(output == input && calls == 3);
}
