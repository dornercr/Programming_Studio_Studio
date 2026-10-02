#include <iostream>
#include <vector>

int manual_sum() {
    int* values = new int[3]{2, 4, 6};
    const int result = values[0] + values[1] + values[2];
    delete[] values;
    values = nullptr;
    return result;
}

int owned_sum() {
    const std::vector<int> values{2, 4, 6};
    int result{};
    for (int value : values) result += value;
    return result;
}

int main() {
    const int a = manual_sum();
    const int b = owned_sum();
    if (a != 12 || b != 12) return 1;
    std::cout << "manual=" << a << " owned=" << b << '\n';
    std::cout << "PASS\n";
}
