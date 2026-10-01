// LAB: Validate a privileged-style range request
// Model a protected service that accepts a length only when a numeric offset and extent fit inside a 16-byte allowed region. Reject both oversized lengths and invalid starting offsets.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <cstddef>
#include <iostream>

int main() {
    const auto kernel_copy = [](std::size_t length) {
        return length <= 8 ? "accepted" : "rejected";
    };
    std::cout << kernel_copy(4) << ' ' << kernel_copy(12) << '\n';
}
