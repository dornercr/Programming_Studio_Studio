// LAB: Distinguish data, waiting, and EOF
// Return a pipe-read state from buffered-byte count and open-writer count. Buffered data must be delivered even after the last writer closes.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    int open_writers = 2;
    bool buffer_empty = true;
    const auto eof = [&] { return buffer_empty && open_writers == 0; };
    --open_writers;
    std::cout << std::boolalpha << eof() << ' ';
    --open_writers;
    std::cout << eof() << '\n';
    assert(eof());
}
