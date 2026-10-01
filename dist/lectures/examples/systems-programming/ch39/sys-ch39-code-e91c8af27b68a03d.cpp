// LAB: Classify termination before reading its value
// Use a tagged status model for normal exit and signal termination. Do not interpret a signal number as an exit code.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <optional>

int main() {
    std::optional<int> child_status = 7;
    assert(child_status.has_value());
    const int exit_code = *child_status;
    child_status.reset();
    std::cout << "exit=" << exit_code << " reaped=" << std::boolalpha
              << !child_status.has_value() << '\n';
}
