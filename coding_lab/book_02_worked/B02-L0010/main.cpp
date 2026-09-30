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

// Original book listing B02-L0010
#include <stdexcept>
#include <vector>
#include <utility>

class Percentage {
    int value_;
    static int validate(int value) {
        if (value < 0 || value > 100) throw std::out_of_range("percentage");
        return value;
    }
public:
    explicit Percentage(int value) : value_(validate(value)) {}
    int value() const noexcept { return value_; }
    void set(int value) { value_ = validate(value); }
};
class Readings {
    std::vector<int> values_;
public:
    explicit Readings(std::vector<int> values) : values_(std::move(values)) {}
    int at(std::size_t index) const { return values_.at(index); }
    void replace(std::size_t index, int value) { values_.at(index) = value; }
};

int main() {
    Percentage p{40};
    CHECK(course::throws<std::out_of_range>([&] { p.set(101); }));
    CHECK(p.value() == 40);
    p.set(0); CHECK(p.value() == 0);
    p.set(100); CHECK(p.value() == 100);
    CHECK(course::throws<std::out_of_range>([] { Percentage invalid{-1}; }));
    const Readings first{{1, 2, 3}};
    auto second = first;
    second.replace(0, 9);
    CHECK(first.at(0) == 1 && second.at(0) == 9);
    course::report();
}
