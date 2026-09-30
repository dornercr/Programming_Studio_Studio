#include <atomic>
#include <cassert>
#include <iostream>
#include <future>

int split_update() {
    std::atomic<int> counter{0};
    const int first = counter.load(), second = counter.load();
    counter.store(first+1); counter.store(second+1);
    return counter.load();
}
int combined_updates() {
    std::atomic<int> counter{0};
    const auto worker = [&] { for(int i=0;i<1000;++i) counter.fetch_add(1); };
    auto first = std::async(std::launch::async,worker);
    auto second = std::async(std::launch::async,worker);
    first.get(); second.get();
    return counter.load();
}

int main() {
    const int split = split_update(), combined = combined_updates();
    assert(split == 1 && combined == 2000);
    std::cout << "split=" << split << " atomic=" << combined << '\n';
}
