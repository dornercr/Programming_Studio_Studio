#include <cassert>
#include <iostream>

bool above_limit(int value, int limit) {
    return value > limit;
}

int main() {
    const int reading = 8;
    std::cout << above_limit(reading, 10) << " "
              << above_limit(reading, 5) << "\n";
    assert(!above_limit(8, 10) && above_limit(8, 5));
}
