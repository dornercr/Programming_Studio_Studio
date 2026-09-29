// LAB: Compare batching with separate transfers
// Under a serial transfer model, compare one 10,000-byte batch with 100 transfers of 100 bytes. Latency is 10 microseconds and bandwidth is 100 bytes per microsecond; reject nonpositive bandwidth.
// This starter verifies the original example. Extend it to satisfy the lab checks.
#include <cassert>
#include <iostream>

int main() {
    const double latency = 10.0;
    const double bytes_per_us = 100.0;
    const auto time = [&](double bytes) { return latency + bytes / bytes_per_us; };
    std::cout << time(100) << ' ' << time(10000) << '\n';
}
