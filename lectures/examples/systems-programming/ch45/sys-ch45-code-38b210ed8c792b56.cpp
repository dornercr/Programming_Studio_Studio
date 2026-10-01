#include <array>
#include <atomic>
#include <iostream>
#include <thread>
int main() {
    std::atomic<int> stock{1};
    std::array<int,2> granted{};
    const auto claim = [&](int id) {
        int expected = 1;
        granted[id] = stock.compare_exchange_strong(expected, 0) ? 1 : 0;
    };
    std::jthread a(claim, 0), b(claim, 1);
    a.join(); b.join();
    std::cout << "grants=" << granted[0]+granted[1]
              << " remaining=" << stock.load() << '\n';
}
