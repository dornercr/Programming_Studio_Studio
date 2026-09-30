// LAB: Compare FIFO and LRU with the same trace
// Simulate a two-entry cache with either first-in-first-out or least-recently-used replacement. Return the final contents from oldest to newest under the selected policy.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    std::vector<char> lru{'A', 'B'};
    lru.erase(lru.begin());
    lru.push_back('A'); // Touch A: it becomes most recently used.
    const char lru_victim = lru.front();
    const char fifo_victim = 'A'; // A arrived first; a hit does not move it.
    std::cout << "LRU=" << lru_victim << " FIFO=" << fifo_victim << '\n';
    assert(lru_victim == 'B');
}
