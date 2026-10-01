#include <cassert>
#include <iostream>
#include <vector>

long long sum_range(const std::vector<int>& values) {
    long long total = 0;
    for(auto cursor = values.begin(); cursor != values.end(); ++cursor)
        total += *cursor;
    return total;
}

int main() {
    assert(sum_range({3,5,7}) == 15);
    assert(sum_range({}) == 0);
    assert(sum_range({-4,4}) == 0);
    std::cout << "nonempty=" << sum_range({3,5,7}) << " empty=" << sum_range({}) << '\n';
}
