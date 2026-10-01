#include <cassert>
#include <iostream>
#include <sstream>

int main() {
    const int first = 7;
    const int second = 5;
    const int total = first + second;
    std::ostringstream message;
    message << "total=" << total;
    std::cout << message.str() << '\n';
    assert(total == 12);
}
