#include <iostream>
#include <vector>
int main() {
    const std::vector<int> readings{4, 9};
    int total = 0;
    // The colon means: visit each value in readings.
    for (int value : readings) {
        total += value;
    }
    std::cout << "total: " << total << '\n';
}
