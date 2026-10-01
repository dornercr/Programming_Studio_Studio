#include <cassert>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> queue;
    queue.push_back(7); // The producer acts before the consumer arrives.
    const bool should_wait = queue.empty();
    std::cout << "wait=" << std::boolalpha << should_wait
              << " value=" << queue.front() << '\n';
    assert(!should_wait);
}
