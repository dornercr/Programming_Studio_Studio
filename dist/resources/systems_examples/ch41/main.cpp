#include <cassert>
#include <iostream>

int main() {
    bool pending = false;
    pending = true;
    pending = true;
    int handled = 0;
    if (pending) { pending = false; ++handled; }
    std::cout << "handled=" << handled << '\n';
    assert(handled == 1);
}
