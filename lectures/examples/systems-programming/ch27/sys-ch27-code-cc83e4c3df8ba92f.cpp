#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
double transfer(double bytes, double latency, double bandwidth) {
    if (!std::isfinite(bytes) || !std::isfinite(latency) || !std::isfinite(bandwidth)
        || bytes < 0 || latency < 0 || bandwidth <= 0)
        throw std::invalid_argument("finite model parameters");
    return latency + bytes/bandwidth;
}
int main() {
    std::cout << "normal=" << transfer(64,2,16) << '\n';
    const double infinity = std::numeric_limits<double>::infinity();
    int rejected = 0;
    try { transfer(infinity,2,16); } catch (const std::invalid_argument&) { ++rejected; }
    try { transfer(64,2,infinity); } catch (const std::invalid_argument&) { ++rejected; }
    std::cout << "nonfinite-rejected=" << rejected << '\n';
}
