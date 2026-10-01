#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

struct Drain { unsigned bytes = 0; bool blocked = false; bool eof = false; };
Drain drain(const std::vector<int>& results) {
    Drain result;
    for(int count : results) {
        if(count == -1) { result.blocked = true; break; }
        if(count == 0) { result.eof = true; break; }
        if(count < 0) throw std::invalid_argument("read error");
        result.bytes += static_cast<unsigned>(count);
    }
    return result;
}

int main() {
    const auto blocked = drain({2,1,-1,9}), ended = drain({2,0,9});
    assert(blocked.bytes == 3 && blocked.blocked && !blocked.eof);
    assert(ended.bytes == 2 && ended.eof && !ended.blocked);
    bool rejected = false;
    try { drain({-2}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "blocked-bytes=" << blocked.bytes << " eof-bytes=" << ended.bytes << '\n';
}
