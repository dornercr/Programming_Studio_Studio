#include <iostream>
struct Registers { int temporary; int preserved; };
int bad_callee(Registers& r) {
    r.temporary = 6; r.preserved = 99;
    return r.temporary;
}
int good_callee(Registers& r) {
    const int saved = r.preserved;
    r.temporary = 6; r.preserved = 99;
    r.preserved = saved;
    return r.temporary;
}
int main() {
    Registers bad{0, 11}, good{0, 11};
    const int a = bad_callee(bad), b = good_callee(good);
    std::cout << "bad result=" << a << " preserved=" << bad.preserved << '\n';
    std::cout << "good result=" << b << " preserved=" << good.preserved << '\n';
}
