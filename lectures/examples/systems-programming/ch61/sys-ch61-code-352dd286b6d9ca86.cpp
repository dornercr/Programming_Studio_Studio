#include <array>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
int main() {
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
    std::array<int, 3> partial{}, lengths{};
    {
        std::vector<std::jthread> workers;
        workers.reserve(3);
        for (std::size_t worker = 0; worker < partial.size(); ++worker) {
            workers.emplace_back([&, worker] {
                const auto first = worker * input.size() / partial.size();
                const auto last = (worker + 1) * input.size() / partial.size();
                lengths[worker] = static_cast<int>(last - first);
                partial[worker] = std::accumulate(input.begin() + first, input.begin() + last, 0);
            });
        }
    } // All jthreads join before results are read.
    const int parallel = std::accumulate(partial.begin(), partial.end(), 0);
    const int sequential = std::accumulate(input.begin(), input.end(), 0);
    std::cout << "chunks=" << lengths[0] << ',' << lengths[1] << ',' << lengths[2] << '\n';
    std::cout << "parallel=" << parallel << " sequential=" << sequential << '\n';
    return parallel == sequential ? 0 : 1;
}
