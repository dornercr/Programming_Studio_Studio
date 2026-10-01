#include <array>
#include <iostream>
#include <numeric>
int main() {
    const std::array<int, 3> latency_ms{10, 10, 10};
    const int wall_ms = 10; // Supplied overlapping interval, not measured.
    const int summed_ms = std::accumulate(latency_ms.begin(), latency_ms.end(), 0);
    std::cout << "mean latency=" << summed_ms / latency_ms.size() << "ms\n";
    std::cout << "wall throughput=" << latency_ms.size() * 1000 / wall_ms << "/s\n";
    std::cout << "wrong serial rate=" << latency_ms.size() * 1000 / summed_ms << "/s\n";
}
