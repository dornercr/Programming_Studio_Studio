#include <iostream>
#include <stdexcept>
int take(int& next) {
    // Borrow one caller-owned counter; do not copy it.
    if (next > 3) throw std::out_of_range("exhausted");
    return next++;
}
int main() {
    int shared_next = 1;
    const int a = take(shared_next);
    const int b = take(shared_next);
    const int c = take(shared_next);
    std::cout << "shared=" << a << ',' << b << ',' << c << '\n';
    try { take(shared_next); }
    catch (const std::out_of_range&) {
        std::cout << "exhausted\n";
    }
}
