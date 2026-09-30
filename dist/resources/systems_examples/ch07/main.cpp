#include <cassert>
#include <iostream>

int main() {
    const int values[]{3, 5, 7};
    const int* cursor = values;
    const int* end = values + 3;
    int total = 0;
    while (cursor != end) {
        total += *cursor;
        ++cursor;
    }
    std::cout << total << '\n';
    assert(total == 15);
}
