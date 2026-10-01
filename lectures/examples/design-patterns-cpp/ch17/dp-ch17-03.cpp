#include <iostream>
int main() {
    int low = 2;
    int& aliased_low = low;
    const int copied_low = low;
    low = 20;
    std::cout << "reference remembers: " << aliased_low << '\n';
    std::cout << "value remembers: " << copied_low << '\n';
    // Using the alias for restoration would merely assign 20 again.
    low = copied_low;
    std::cout << "restored: " << low << '\n';
}
