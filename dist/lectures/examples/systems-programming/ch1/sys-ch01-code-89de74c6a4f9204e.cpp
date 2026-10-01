// LAB: Separate calculation from presentation
// Extract a reusable sum operation and a formatter. Changing the label must not change the numeric result. Inputs are small integers whose sum fits in int.
// This starter verifies the original example. Extend it to satisfy the lab checks.
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
