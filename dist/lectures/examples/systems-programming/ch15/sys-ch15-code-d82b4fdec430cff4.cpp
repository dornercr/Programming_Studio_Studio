#include <iostream>
void show(unsigned a, unsigned b) {
    const unsigned wide = a + b; // Inputs below 256 keep this safely small.
    const unsigned result = wide & 255u;
    const bool carry = wide > 255u;
    const bool overflow = ((~(a ^ b) & (a ^ result)) & 128u) != 0;
    std::cout << a << '+' << b << " result=" << result
              << " carry=" << carry << " signed-overflow=" << overflow << '\n';
}
int main() {
    std::cout << std::boolalpha;
    show(250, 10);
    show(127, 1);
}
