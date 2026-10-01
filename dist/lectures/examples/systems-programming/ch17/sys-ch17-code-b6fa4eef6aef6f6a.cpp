#include <cassert>
#include <iostream>

int main() {
    const auto helper = [](int argument) { return argument * 2; };
    const int saved = 10;
    const int returned = helper(3);
    std::cout << saved + returned << '\n';
    assert(saved + returned == 16);
}
