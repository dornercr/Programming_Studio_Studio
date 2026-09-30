// LAB: Grow a collection under a hard limit
// Append an element only when a vector is below a fixed maximum size. Rejection must leave the vector unchanged. Do not retain an element pointer while growing it.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> values;
    values.reserve(1);
    values.push_back(4);
    values.push_back(6);
    values.push_back(8);
    const int total = std::accumulate(values.begin(), values.end(), 0);
    std::cout << values.size() << ' ' << total << '\n';
    assert(values.size() == 3 && total == 18);
}
