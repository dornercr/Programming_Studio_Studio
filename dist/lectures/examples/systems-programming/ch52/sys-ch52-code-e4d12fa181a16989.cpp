#include <iostream>
int main() {
    const unsigned limit = 2;
    unsigned active = 1;
    // Explicit serialized interleaving: no threads and no data race.
    const bool first_yes = active < limit;
    const bool second_yes = active < limit;
    if (first_yes) ++active;
    if (second_yes) ++active;
    std::cout << "active=" << active << " limit=" << limit << '\n';
    std::cout << "within-budget=" << std::boolalpha << (active <= limit) << '\n';
}
