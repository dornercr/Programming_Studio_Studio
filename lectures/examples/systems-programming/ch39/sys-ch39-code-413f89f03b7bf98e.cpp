#include <cassert>
#include <iostream>
#include <string>

enum class Termination { Exited, Signaled };
struct Status { Termination kind; int value; };
std::string describe(Status status) {
    return (status.kind == Termination::Exited ? "exit=" : "signal=") + std::to_string(status.value);
}

int main() {
    assert(describe({Termination::Exited,7}) == "exit=7");
    assert(describe({Termination::Signaled,15}) == "signal=15");
    std::cout << describe({Termination::Exited,7}) << ' ' << describe({Termination::Signaled,15}) << '\n';
}
