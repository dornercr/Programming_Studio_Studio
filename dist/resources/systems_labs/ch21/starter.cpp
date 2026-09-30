// LAB: Enforce scheduler state transitions
// Use an enum to model Ready, Running, and Waiting. Permit dispatch, blocking, and wakeup only from their valid starting states.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string a = "running";
    std::string b = "ready";
    a = "waiting";
    b = "running";
    std::cout << "A=" << a << " B=" << b << '\n';
    a = "ready";
    std::cout << "A=" << a << " B=" << b << '\n';
}
