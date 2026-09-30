#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

std::vector<float> reference_transform(const std::vector<float>& input) {
    std::vector<float> output(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] * 2.0f + 1.0f;
    }
    return output;
}

bool close_enough(float a, float b, float tolerance = 1e-5f) {
    return std::abs(a - b) <= tolerance;
}

int main() {
    const std::vector<float> input{1.0f, 2.0f, 3.0f, 4.0f};
    const auto expected = reference_transform(input);
    const auto observed = reference_transform(input); // replace with optimized path

    if (expected.size() != observed.size()) return 2;
    for (std::size_t i = 0; i < expected.size(); ++i) {
        if (!close_enough(expected[i], observed[i])) return 3;
    }
    std::cout << "verified " << observed.size() << " values\n";
}
