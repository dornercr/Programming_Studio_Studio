#include <array>
#include <iostream>
#include <thread>
#include <vector>
int main() {
    std::array<int,3> results{};
    {
        std::vector<std::jthread> workers;
        workers.reserve(3);
        for (int id = 0; id < 3; ++id)
            workers.emplace_back([id, &results] { results[id] = 10 + id; });
    } // Every jthread has joined before results are read.
    std::cout << results[0] << ' ' << results[1] << ' ' << results[2] << '\n';
}
