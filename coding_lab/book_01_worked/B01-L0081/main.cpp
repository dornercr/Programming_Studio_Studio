#include <array>
#include <iostream>
#include <vector>

int main() {
    std::array<int, 3> calibration{1, 2, 3};
    for (int& value : calibration) value *= 2;
    std::vector<int> readings;
    readings.reserve(8);
    if (!readings.empty() || readings.capacity() < 8) return 1;
    readings.push_back(10);
    readings.push_back(20);
    readings.push_back(30);
    readings.pop_back();
    if (readings.size() != 2 || readings.at(1) != 20) return 2;
    if (calibration[0] != 2 || calibration[2] != 6) return 3;
    std::cout << "size=" << readings.size()
              << " capacity=" << readings.capacity() << '\n';
    std::cout << "PASS\n";
}
