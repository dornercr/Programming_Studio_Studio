#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<int> values{8, 9, 10};
    try {
        std::cout << values.at(3) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "out of range\n";
    }
    assert(values.size() == 3);
}
