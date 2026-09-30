#include <cassert>
#include <iostream>

int main() {
    const double latency = 10.0;
    const double bytes_per_us = 100.0;
    const auto time = [&](double bytes) { return latency + bytes / bytes_per_us; };
    std::cout << time(100) << ' ' << time(10000) << '\n';
}
