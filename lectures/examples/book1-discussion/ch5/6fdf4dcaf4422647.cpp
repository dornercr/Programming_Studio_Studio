#include <iostream>
int main() {
    int a = 2, b = 3, c = 4;
    int implicit = a + b * c;
    int grouped = (a + b) * c;
    std::cout << implicit << ' ' << grouped << '\n';
}
