// LAB: Make copying and borrowing visible
// Write one function that returns a changed copy and one that changes the caller’s vector. Reject an empty vector before accessing its first element.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> readings{2, 4};
    auto copy = readings;
    auto& borrowed = readings;
    copy[0] = 9;
    borrowed[1] = 7;
    for (const int value : readings) std::cout << value << ' ';
    std::cout << '\n';
    assert(readings[0] == 2 && readings[1] == 7);
}
