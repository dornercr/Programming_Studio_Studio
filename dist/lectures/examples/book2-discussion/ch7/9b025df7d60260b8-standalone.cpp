// Exact original book listing B02-L0110
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

// Original book listing B02-L0032
#include <compare>
#include <limits>
#include <istream>
#include <ostream>
#include <sstream>

static_assert(std::numeric_limits<int>::max() >= 2000000);

class Quantity {
    int value_{};
public:
    explicit Quantity(int value = 0) : value_(value) {
        if (value < -1000000 || value > 1000000) throw std::out_of_range("quantity");
    }
    int value() const noexcept { return value_; }
    auto operator<=>(const Quantity&) const = default;
    friend Quantity operator+(Quantity left, Quantity right) {
        // The constrained operands make the intermediate addition representable.
        return Quantity{left.value_ + right.value_};
    }
    friend std::ostream& operator<<(std::ostream& out, Quantity value) {
        return out << value.value_;
    }
    friend std::istream& operator>>(std::istream& in, Quantity& value) {
        int candidate{};
        if (in >> candidate) {
            if (candidate < -1000000 || candidate > 1000000) {
                in.setstate(std::ios::failbit);
            } else { value.value_ = candidate; }
        }
        return in;
    }
};

int main() {
    CHECK(Quantity{2} + Quantity{3} == Quantity{5});
    CHECK(Quantity{2} < Quantity{3});
    CHECK(course::throws<std::out_of_range>([] {
        (void)(Quantity{1000000} + Quantity{1});
    }));
    Quantity value{7};
    std::istringstream valid{"12"}; valid >> value;
    CHECK(valid && value == Quantity{12});
    std::istringstream invalid{"1000001"}; invalid >> value;
    CHECK(invalid.fail() && value == Quantity{12});
    std::ostringstream output; output << value;
    CHECK(output.str() == "12");
    course::report();
}
