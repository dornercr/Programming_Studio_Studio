#include "course_test.hpp"
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
