#include <iostream>
int main() {
    for (int n = 1; n <= 10; ++n) {
        if (n % 2 == 0) continue;
        if (n > 7) break;
        std::cout << n << ' ';
    }
    std::cout << '\n';
}
