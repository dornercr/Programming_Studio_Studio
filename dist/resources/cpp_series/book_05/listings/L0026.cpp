#ifndef HARBOR_VENDOR_BOUNDED_HPP
#define HARBOR_VENDOR_BOUNDED_HPP
#include <stdexcept>
namespace harbor_vendor {
inline constexpr const char* version = "1.0.0-teaching";
inline int bounded(int value, int low, int high) {
    if (low > high) throw std::invalid_argument("reversed bounds");
    return value < low ? low : (value > high ? high : value);
}
}
#endif
