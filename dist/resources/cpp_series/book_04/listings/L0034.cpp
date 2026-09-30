#include <iostream>

int add(int a, int b) { return a + b; }

struct Scale {
    int factor{3};
    int apply(int x) const { return x * factor; }
};

int main() {
    int (*operation)(int,int) = &add;
    int (Scale::*member)(int) const = &Scale::apply;
    Scale s;
    std::cout << operation(2,4) << ' ' << (s.*member)(5) << "\n";
}
