#include <cstddef>
#include <iostream>
#include <vector>
int main() {
    std::vector<int> values{4, 9};
    std::size_t generation = 2;
    const auto saved_generation = generation;
    const std::size_t index = 0;
    values.push_back(12);
    ++generation;
    // Index access is safe; it does not enforce a cursor policy.
    std::cout << std::boolalpha;
    std::cout << "index in range: " << (index < values.size()) << '\n';
    std::cout << "value: " << values.at(index) << '\n';
    std::cout << "generation current: "
              << (saved_generation == generation) << '\n';
}
