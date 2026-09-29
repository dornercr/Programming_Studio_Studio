#include "scale.hpp"
#include <limits>
#include <stdexcept>
namespace harbor {
int scale(int value, int factor) {
    if (value < -10000 || value > 10000 || factor < -10000 || factor > 10000)
        throw std::invalid_argument("scale domain");
    const long long result = static_cast<long long>(value) * factor;
    if (result < std::numeric_limits<int>::min() || result > std::numeric_limits<int>::max())
        throw std::overflow_error("int result");
    return static_cast<int>(result);
}
}
