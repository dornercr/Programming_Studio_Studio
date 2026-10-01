#include <iostream>
int global_zero;
void visit() {
    int local = 0;
    static int retained = 0;
    ++local;
    ++retained;
    std::cout << "local=" << local << " retained=" << retained << '\n';
}
int main() {
    std::cout << "global zero=" << global_zero << '\n';
    visit();
    visit();
}
