#include <cstddef>
#include <iostream>
#include <vector>
int main() {
    std::vector<int> values{4, 8};
    const std::size_t saved_index = 1;
    const int snapshot = values[saved_index];
    values.reserve(values.capacity() + 1); // Forces growth of capacity.
    values.push_back(12);
    // Reacquire access after growth; never use an old element pointer.
    const int* current = &values.at(saved_index);
    std::cout << "saved value=" << snapshot << '\n';
    std::cout << "reacquired value=" << *current << '\n';
    std::cout << "elements=" << values.size() << '\n';
}
