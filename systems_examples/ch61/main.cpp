#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

int main() {
    const std::vector<int> readings{2, 4, 6, 8, 10, 12};
    std::array<int, 2> partial{0, 0};
    const auto work = [&](std::size_t worker) {
        const std::size_t begin = worker * 3;
        partial[worker] = std::accumulate(readings.begin() + begin,
            readings.begin() + begin + 3, 0);
    };
    std::thread first(work, 0);
    std::thread second(work, 1);
    first.join(); second.join();
    const int total = partial[0] + partial[1];
    const int reference = std::accumulate(readings.begin(), readings.end(), 0);
    assert(total == reference);
    std::cout << "parallel=" << total << " reference=" << reference << '\n';
}
