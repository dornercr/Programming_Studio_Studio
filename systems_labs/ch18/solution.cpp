#include <cassert>
#include <iostream>

int write_then_read(int& a, const int& b) { a = 9; return b; }
int cached_before(int& a, const int& b) { const int old = b; a = 9; return old; }

int main() {
    int a = 4, b = 6;
    assert(write_then_read(a,b) == 6 && a == 9);
    int x = 4;
    const int correct = write_then_read(x,x);
    x = 4;
    const int changed = cached_before(x,x);
    assert(correct == 9 && changed == 4);
    std::cout << "ordered=" << correct << " reordered=" << changed << '\n';
}
