#include <array>
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono_literals;
    const std::array durations{8ms,9ms,14ms,7ms,8ms};
    constexpr auto deadline=10ms;unsigned misses=0;
    std::chrono::milliseconds total{};
    for(auto d:durations){total+=d;if(d>deadline)++misses;}
    std::cout << "mean_ms=" << double(total.count())/durations.size()
              << " misses=" << misses << '/' << durations.size() << '\n';
}
