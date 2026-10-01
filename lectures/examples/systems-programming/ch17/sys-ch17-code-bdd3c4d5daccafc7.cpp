#include <cassert>
#include <iostream>

int helper(int argument) { return argument*2; }
int caller(int saved, int argument) {
    const int returned = helper(argument);
    return saved+returned;
}

int main() {
    assert(caller(10,3) == 16 && caller(10,0) == 10 && caller(10,-3) == 4);
    std::cout << caller(10,3) << ' ' << caller(10,0) << ' ' << caller(10,-3) << '\n';
}
