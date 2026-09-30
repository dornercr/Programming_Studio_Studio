#include "calibration.hpp"
#include <cmath>
#include <stdexcept>
namespace harbor {
double calibrate(double raw, double gain, double offset) {
    if (!std::isfinite(raw) || !std::isfinite(gain) || !std::isfinite(offset))
        throw std::invalid_argument("nonfinite calibration input");
    const double result = raw * gain + offset;
    if (!std::isfinite(result)) throw std::overflow_error("calibration overflow");
    return result;
}
}
