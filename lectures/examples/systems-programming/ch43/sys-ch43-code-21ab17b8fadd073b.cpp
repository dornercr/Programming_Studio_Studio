#include <cassert>
#include <iostream>

int main() {
    int counter = 0;
    const int a_read = counter;
    const int b_read = counter;
    counter = a_read + 1;
    counter = b_read + 1;
    std::cout << counter << '\n';
    assert(counter == 1);
}
