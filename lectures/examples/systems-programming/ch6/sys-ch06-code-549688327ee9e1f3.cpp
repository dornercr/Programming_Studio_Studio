// LAB: Compute a checked row-major index
// Map a row and column into a flat vector with a known number of columns. Check dimensions and bounds before accessing the data.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const int values[]{10, 20, 30};
    const int* first = values;
    const int* third = first + 2;
    std::cout << *third << ' ' << (third - first) << '\n';
    assert(*third == 30 && third - first == 2);
}
