#include <iostream>
#include <vector>

int total(const std::vector<int>& values) {
    int result{};
    for (int value : values) result += value;
    return result;
}

void apply_offset(std::vector<int>& values, int offset) {
    for (int& value : values) value += offset;
}

int main() {
    std::vector<int> readings{1, 2, 3};
    const int before = total(readings);
    apply_offset(readings, 2);
    const int after = total(readings);
    int& first = readings.at(0);
    first = 10;
    if (before != 6 || after != 12 || readings[0] != 10) return 1;
    std::cout << "before=" << before << " after=" << after << '\n';
    std::cout << "PASS\n";
}
