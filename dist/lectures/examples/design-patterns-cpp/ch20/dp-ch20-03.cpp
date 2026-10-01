#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
int summarize(const std::vector<int>& data, bool highest) {
    if (data.empty() || data.size() > 1000)
        throw std::invalid_argument("size");
    for (int sample : data)
        if (sample < 0 || sample > 1000)
            throw std::invalid_argument("sample");
    if (highest)
        return *std::max_element(data.begin(), data.end());
    int sum = 0;
    for (int sample : data) sum += sample;
    return sum / static_cast<int>(data.size());
}
int main() {
    const std::vector<int> data{2, 4, 9};
    std::cout << "average=" << summarize(data, false) << '\n';
    std::cout << "highest=" << summarize(data, true) << '\n';
}
