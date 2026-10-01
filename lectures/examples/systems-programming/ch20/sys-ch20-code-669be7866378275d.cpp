#include <cassert>
#include <iostream>

bool load(bool permitted, bool& present, int& pc, int& faults) {
    if(!permitted) { ++faults; return false; }
    if(!present) { ++faults; present = true; }
    ++pc;
    return true;
}

int main() {
    bool present = false; int pc = 8, faults = 0;
    assert(load(true,present,pc,faults) && pc == 9 && faults == 1);
    assert(load(true,present,pc,faults) && pc == 10 && faults == 1);
    bool absent = false; int stopped = 8, denied = 0;
    assert(!load(false,absent,stopped,denied) && stopped == 8 && !absent);
    std::cout << "completed-pc=" << pc << " denied-pc=" << stopped << '\n';
}
