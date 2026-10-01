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
