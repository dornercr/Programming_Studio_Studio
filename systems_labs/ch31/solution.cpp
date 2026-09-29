#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<char> simulate(const std::vector<char>& trace, bool lru) {
    std::vector<char> resident;
    for(char block : trace) {
        const auto found = std::find(resident.begin(),resident.end(),block);
        if(found != resident.end()) {
            if(lru) { resident.erase(found); resident.push_back(block); }
            continue;
        }
        if(resident.size() == 2) resident.erase(resident.begin());
        resident.push_back(block);
    }
    return resident;
}

int main() {
    const auto fifo = simulate({'A','B','A','C'},false);
    const auto lru = simulate({'A','B','A','C'},true);
    assert((fifo == std::vector<char>{'B','C'}));
    assert((lru == std::vector<char>{'A','C'}));
    assert(simulate({'A','A','A'},true).size() == 1);
    std::cout << "FIFO=" << fifo[0] << fifo[1] << " LRU=" << lru[0] << lru[1] << '\n';
}
