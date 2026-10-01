#include <array>
#include <cassert>
#include <iostream>
#include <numeric>

int main() {
    const std::array<double, 3> milliseconds{2, 8, 5};
    const double elapsed = std::accumulate(milliseconds.begin(), milliseconds.end(), 0.0);
    const double mean = elapsed / milliseconds.size();
    const double throughput = milliseconds.size() / (elapsed / 1000.0);
    std::cout << "mean=" << mean << "ms throughput=" << throughput << "/s\n";
}
