// LAB: Parse complete frames without losing the next one
// Parse a one-byte length followed by that many payload bytes, with maximum length eight. Report incomplete input separately from an invalid length, and return the number of consumed bytes.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<unsigned char> received{3, 'c'};
    received.insert(received.end(), {'a', 't'});
    const std::size_t length = received[0];
    assert(length <= 8 && received.size() >= 1 + length);
    const std::string message(received.begin() + 1, received.begin() + 1 + length);
    std::cout << message << '\n';
}
