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
