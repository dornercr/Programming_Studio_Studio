#include <iostream>
#include <stdexcept>
struct Bolts {
    int count;
    explicit Bolts(int value) : count(value) {
        if (value < 0 || value > 100)
            throw std::out_of_range("bolt count");
    }
};
struct Cable {
    int metres;
    explicit Cable(int value) : metres(value) {
        if (value < 0 || value > 100)
            throw std::out_of_range("cable length");
    }
};
int price(const Bolts& bolts) { return bolts.count * 5; }
int price(const Cable& cable) { return cable.metres * 40; }
int main() {
    const Bolts bolts(4);
    const Cable cable(3);
    std::cout << "total=" << price(bolts) + price(cable) << '\n';
}
