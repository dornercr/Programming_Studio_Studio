#include <cassert>
#include <iostream>

int main() {
    int x = 4;
    int& a = x;
    int& b = x;
    const int remembered = b;
    a = 9;
    const int after = b;
    std::cout << remembered << ' ' << after << '\n';
    assert(remembered == 4 && after == 9);
}
