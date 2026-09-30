#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

std::unique_ptr<int[]> makeSequence(std::size_t count) {
    if (count > 8) throw std::invalid_argument("range");
    if (count == 0) return {};
    auto result = std::make_unique<int[]>(count);
    for (std::size_t i = 0; i < count; ++i) result[i] = static_cast<int>(i) + 1;
    return result; // Transfer sole ownership to the caller.
}

int main() {
    auto p=makeSequence(3);
    std::cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<"\n";
    }
