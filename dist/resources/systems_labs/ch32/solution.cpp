#include <cassert>
#include <iostream>
#include <cmath>
#include <stdexcept>

double amat(double h1, double m1, double h2, double m2, double memory) {
    if(!(h1 >= 0 && h2 >= 0 && memory >= 0 && m1 >= 0 && m1 <= 1 && m2 >= 0 && m2 <= 1))
        throw std::invalid_argument("cost model");
    return h1+m1*(h2+m2*memory);
}

int main() {
    const double result = amat(1,.1,5,.2,50);
    assert(std::abs(result-2.5) < 1e-12);
    assert(amat(1,0,5,.2,50) == 1);
    bool rejected = false;
    try { amat(1,.1,5,2,50); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "AMAT=" << result << "ns\n";
}
