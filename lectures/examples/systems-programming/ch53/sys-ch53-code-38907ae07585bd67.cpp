// LAB: Model nonblocking drain outcomes
// Consume a supplied trace of read results: positive counts add bytes, -1 means would-block in this model, and zero means EOF. Reject other negative values.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <array>
#include <cassert>
#include <iostream>

int main() {
    const std::array<int, 3> results{2, 1, -1}; // -1 models would-block, not EOF.
    int buffered = 0;
    for (int result : results) {
        if (result == -1) break;
        buffered += result;
    }
    std::cout << "buffered=" << buffered << " keep-open\n";
    assert(buffered == 3);
}
