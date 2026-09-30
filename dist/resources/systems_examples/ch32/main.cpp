#include <cassert>
#include <iostream>

int main() {
    const double hit_time = 1;
    const double miss_rate = 0.05;
    const double miss_penalty = 40;
    const double amat = hit_time + miss_rate * miss_penalty;
    std::cout << amat << " ns\n";
    assert(amat == 3);
}
