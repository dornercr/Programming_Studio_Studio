#include <iostream>
int main() {
    const int busy_items = 8;
    const int quiet_items = 1;
    const int service_budget = 2;
    const int unbounded_position = busy_items + quiet_items;
    const int budgeted_position = service_budget + quiet_items;
    std::cout << "unbounded quiet-position=" << unbounded_position << '\n';
    std::cout << "budgeted quiet-position=" << budgeted_position << '\n';
}
