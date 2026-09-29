#include <cassert>
#include <iostream>

int main() {
    const int values[]{10, 20, 30};
    const int* first = values;
    const int* third = first + 2;
    std::cout << *third << ' ' << (third - first) << '\n';
    assert(*third == 30 && third - first == 2);
}
