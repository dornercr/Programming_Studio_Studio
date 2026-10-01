#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
int main() {
    std::vector<int> milliseconds{1, 1, 1, 1, 1, 1, 1, 1, 1, 91};
    const int total = std::accumulate(milliseconds.begin(), milliseconds.end(), 0);
    std::sort(milliseconds.begin(), milliseconds.end());
    const auto rank = static_cast<std::size_t>(std::ceil(0.95 * milliseconds.size()));
    std::cout << "mean=" << total / milliseconds.size()
              << " median=" << milliseconds[4]
              << " nearest-rank-p95=" << milliseconds[rank - 1] << "ms\n";
}
