// LAB: Enforce an admission budget
// Create a gate with a maximum active count. Admit only below the limit, release one existing permit at a time, and reject a release when none is held.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const unsigned budget_mib = 64;
    const unsigned per_connection_mib = 2;
    const unsigned limit = budget_mib / per_connection_mib;
    const unsigned active = 32;
    std::cout << "limit=" << limit << " admit=" << std::boolalpha
              << (active < limit) << '\n';
    assert(limit == 32 && !(active < limit));
}
