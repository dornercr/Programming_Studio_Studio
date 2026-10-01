#include <iostream>
#define SMALL(x) ((x) >= 0 && (x) < 10)
int observe(int& calls) { ++calls; return 6; }
bool small(int value) { return value >= 0 && value < 10; }
int main() {
    int calls = 0;
    const bool macro = SMALL(observe(calls));
    std::cout << std::boolalpha << "macro=" << macro
              << " calls=" << calls << '\n';
    calls = 0;
    const bool function = small(observe(calls));
    std::cout << "function=" << function << " calls=" << calls << '\n';
}
