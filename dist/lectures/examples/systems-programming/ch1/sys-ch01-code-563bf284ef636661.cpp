#include <iostream>
int observe(int& calls) {
    ++calls;
    return 6;
}
int square(int value) { return value * value; }
int main() {
    int calls = 0;
    const int result = square(observe(calls));
    std::cout << "result=" << result << " calls=" << calls << '\n';
}
