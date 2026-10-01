#include <atomic>
#include <cassert>
#include <iostream>

int main() {
    std::atomic<int> counter{0};
    const int a = counter.load();
    const int b = counter.load();
    counter.store(a + 1);
    counter.store(b + 1);
    const int split_result = counter.load();
    counter.store(0);
    counter.fetch_add(1);
    counter.fetch_add(1);
    std::cout << split_result << ' ' << counter.load() << '\n';
    assert(split_result == 1 && counter.load() == 2);
}
