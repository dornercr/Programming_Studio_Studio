#include "summary.hpp"
#include <stdexcept>
namespace harbor {
Summary summarize(std::span<const int> values) {
    if (values.empty() || values.size() > 1000000)
        throw std::invalid_argument("sample count");
    long long total = 0;
    for (int value : values) {
        if (value < -1000000 || value > 1000000)
            throw std::invalid_argument("sample magnitude");
        total += value;
    }
    return {values.size(), total, static_cast<double>(total)/static_cast<double>(values.size())};
}
}
