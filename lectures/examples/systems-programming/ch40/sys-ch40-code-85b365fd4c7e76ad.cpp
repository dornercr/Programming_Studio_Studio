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
