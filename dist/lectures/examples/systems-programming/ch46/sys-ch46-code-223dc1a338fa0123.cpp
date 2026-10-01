#include <algorithm>
#include <array>
#include <atomic>
#include <iostream>
#include <thread>
int main() {
    std::atomic<int> next{10};
    std::array<int,3> tickets{};
    const auto allocate = [&](int id) {
        tickets[id] = next.fetch_add(1, std::memory_order_relaxed);
    };
    std::jthread a(allocate,0), b(allocate,1), c(allocate,2);
    a.join(); b.join(); c.join();
    std::sort(tickets.begin(), tickets.end());
    std::cout << "tickets=" << tickets[0] << ',' << tickets[1] << ',' << tickets[2]
              << " next=" << next.load() << '\n';
}
