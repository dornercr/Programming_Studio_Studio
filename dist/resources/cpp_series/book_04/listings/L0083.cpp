#include "check.hpp"
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <span>
#include <stdexcept>
#include <thread>
#include <vector>

long long parallel_sum(std::span<const int> values, std::size_t requested) {
    if (requested == 0 || requested > 64 || values.size() > 1000000)
        throw std::invalid_argument("work bounds");
    for (int value : values)
        if (value < -1000000 || value > 1000000)
            throw std::invalid_argument("value bounds");
    if (values.empty()) return 0;
    const auto workers = std::min(requested, values.size());
    std::vector<long long> partial(workers, 0);
    std::vector<std::jthread> threads;
    threads.reserve(workers);
    for (std::size_t id = 0; id < workers; ++id) {
        const auto base = values.size() / workers;
        const auto extra = values.size() % workers;
        const auto begin = id * base + std::min(id, extra);
        const auto end = begin + base + (id < extra ? 1 : 0);
        threads.emplace_back([&, id, begin, end] {
            long long sum = 0;
            for (auto i = begin; i < end; ++i) sum += values[i];
            partial[id] = sum;
        });
    }
    threads.clear(); // jthread destruction joins before partial is read.
    return std::accumulate(partial.begin(), partial.end(), 0LL);
}
int main() {
    int observed = 0;
    std::thread simple([&] { observed = 7; });
    simple.join();
    CHECK(observed == 7);
    std::vector<int> values(1003);
    std::iota(values.begin(), values.end(), -501);
    const auto expected = std::accumulate(values.begin(), values.end(), 0LL);
    for (std::size_t workers : {1U, 2U, 3U, 8U, 64U})
        CHECK(parallel_sum(values, workers) == expected);
    CHECK(parallel_sum({}, 4) == 0);
    const std::vector<int> tiny{5, 7};
    CHECK(parallel_sum(tiny, 8) == 12);
    std::cout << "PASS\n";
}
