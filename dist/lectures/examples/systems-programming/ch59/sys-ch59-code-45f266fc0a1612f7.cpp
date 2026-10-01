// LAB: Validate then commit a configuration update
// Accept exactly version=N with decimal N in [1,99]. Parse into temporary state and change the live string only after complete validation.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string live = "version=1";
    std::string candidate = "broken";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    candidate = "version=2";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    assert(live == "version=2");
}
