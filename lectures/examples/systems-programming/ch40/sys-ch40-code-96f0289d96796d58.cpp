#include <cassert>
#include <iostream>

enum class ReadState { Data, Wait, End };
ReadState read_state(unsigned bytes, unsigned writers) {
    if(bytes != 0) return ReadState::Data;
    return writers == 0 ? ReadState::End : ReadState::Wait;
}

int main() {
    assert(read_state(2,0) == ReadState::Data);
    assert(read_state(0,1) == ReadState::Wait);
    assert(read_state(0,0) == ReadState::End);
    std::cout << "buffered=data empty-with-writer=wait drained-and-closed=EOF\n";
}
