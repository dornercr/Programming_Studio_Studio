#include <iostream>
int quotient(int divisor, int& calls) {
    ++calls;
    return 10 / divisor; // Caller guarantees a nonzero divisor here.
}
int main() {
    int calls = 0;
    int divisor = 0;
    int result = divisor != 0 ? quotient(divisor, calls) : 0;
    std::cout << "result=" << result << " calls=" << calls << '\n';
    divisor = 2;
    result = divisor != 0 ? quotient(divisor, calls) : 0;
    std::cout << "result=" << result << " calls=" << calls << '\n';
}
