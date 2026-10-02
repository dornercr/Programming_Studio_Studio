#ifndef HARBOR_CHECKED_NUMERIC_HPP
#define HARBOR_CHECKED_NUMERIC_HPP
#include <concepts>
#include <limits>
#include <span>
#include <stdexcept>
#include <type_traits>

namespace harbor::numeric {
template<class T>
concept Integer = std::integral<T> && !std::same_as<std::remove_cv_t<T>, bool>;

template<Integer T>
constexpr T checked_add(T left, T right) {
    constexpr T high = std::numeric_limits<T>::max();
    if constexpr (std::is_signed_v<T>) {
        constexpr T low = std::numeric_limits<T>::min();
        if ((right > 0 && left > high - right) ||
            (right < 0 && left < low - right)) {
            throw std::overflow_error("sum is not representable");
        }
    } else if (left > high - right) {
        throw std::overflow_error("sum is not representable");
    }
    return static_cast<T>(left + right);
}

template<Integer T>
T checked_total(std::span<const T> values) {
    T result{};
    for (T value : values) result = checked_add(result, value);
    return result;
}
}
#endif
