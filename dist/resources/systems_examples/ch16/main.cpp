#include <cassert>
#include <iostream>

int main() {
    int sum = 0;
    int i = 0;
    while (i < 4) {
        sum += i;
        ++i;
    }
    std::cout << "i=" << i << " sum=" << sum << '\n';
    assert(i == 4 && sum == 6);
}
