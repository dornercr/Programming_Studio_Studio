#include <cassert>
#include <iostream>
#include <stdexcept>

double transfer(double bytes, double latency, double bandwidth) {
    if(!(bytes >= 0) || !(latency >= 0) || !(bandwidth > 0))
        throw std::invalid_argument("model parameters");
    return latency+bytes/bandwidth;
}

int main() {
    const double batch = transfer(10000,10,100);
    const double separate = 100*transfer(100,10,100);
    assert(batch == 110 && separate == 1100);
    int rejected = 0;
    try { transfer(1,10,0); } catch(const std::invalid_argument&) { ++rejected; }
    try { transfer(1,10,-1); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "batch=" << batch << "us separate=" << separate << "us\n";
}
