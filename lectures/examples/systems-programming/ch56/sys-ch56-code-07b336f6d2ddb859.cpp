#include <cassert>
#include <iostream>
#include <cmath>
#include <stdexcept>

double speedup(double fraction, double local) {
    if(!std::isfinite(fraction) || fraction < 0 || fraction > 1 || !std::isfinite(local) || local <= 0)
        throw std::invalid_argument("speedup model");
    return 1/((1-fraction)+fraction/local);
}

int main() {
    assert(std::abs(speedup(.4,2)-1.25) < 1e-12);
    assert(speedup(0,2) == 1 && speedup(1,2) == 2 && speedup(1,.5) == .5);
    int rejected = 0;
    try { speedup(1.1,2); } catch(const std::invalid_argument&) { ++rejected; }
    try { speedup(.4,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "whole=" << speedup(.4,2) << " unchanged=" << speedup(0,2) << '\n';
}
