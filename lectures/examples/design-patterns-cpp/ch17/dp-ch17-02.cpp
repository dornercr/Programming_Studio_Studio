#include <iostream>
int main() {
    int low = 2;
    int high = 5;
    // Copies retain this moment, independently of later edits.
    const int old_low = low;
    const int old_high = high;
    low = 20;
    high = 40;
    std::cout << "changed: " << low << ':' << high << '\n';
    low = old_low;
    high = old_high;
    std::cout << "restored: " << low << ':' << high << '\n';
}
