#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include <stdexcept>

bool close(double a, double b, double absolute, double relative) {
    if(!(absolute >= 0) || !(relative >= 0)) throw std::invalid_argument("tolerance");
    if(a == b) return true;
    if(!std::isfinite(a) || !std::isfinite(b)) return false;
    const double scale = std::max(std::abs(a),std::abs(b));
    return std::abs(a-b) <= std::max(absolute,relative*scale);
}

int main() {
    assert(close(0.1+0.2,0.3,1e-12,1e-12));
    assert(close(0,1e-14,1e-12,0));
    assert(!close(1000000,1000010,0,1e-8));
    assert(!close(std::numeric_limits<double>::quiet_NaN(),0,1,1));
    bool rejected = false;
    try { close(1,1,-1,0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "decimal=close near-zero=close far=distinct\n";
}
