#include <iostream>
#include <stdexcept>
int take(int& next) {
    if (next > 3) throw std::out_of_range("exhausted");
    return next++;
}
int main() {
    int desk_a_next = 1;
    int desk_b_next = 1;
    const int a = take(desk_a_next);
    const int b = take(desk_b_next);
    std::cout << "separate desks=" << a << ',' << b << '\n';
    std::cout << "duplicate=" << std::boolalpha << (a == b) << '\n';
    int one_next = 1; // Repair: give both callers the same state.
    const int shared_a = take(one_next);
    const int shared_b = take(one_next);
    std::cout << "shared desks=" << shared_a << ',' << shared_b << '\n';
}
