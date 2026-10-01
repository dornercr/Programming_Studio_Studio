#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<int> durations{10, 20, 5};
    int total = 0;
    for (int n : durations) { // Copy each small integer, not the container.
        if (n < 0 || n > 10000 - total) {
            throw std::invalid_argument("duration or budget");
        }
        total += n;
    }
    std::cout << "flat total=" << total << '\n';
}
