#include <iostream>
struct Source { int hour = 8; int reads = 0; };
int read_hour(Source& source) {
    // Borrow the same object; this is a simulated service read.
    ++source.reads;
    return source.hour;
}
int main() {
    Source source;
    std::cout << "first=" << read_hour(source) << '\n';
    source.hour = 9;
    std::cout << "second=" << read_hour(source) << '\n';
    std::cout << "reads=" << source.reads << '\n';
}
