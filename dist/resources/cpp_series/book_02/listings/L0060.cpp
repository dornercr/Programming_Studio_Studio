#include "course_test.hpp"
#include <algorithm>
#include <vector>

auto make_counter(int initial) {
    return [value = initial]() mutable { return ++value; };
}

int main() {
    int cutoff{3};
    const auto fixed = [cutoff](int value) { return value >= cutoff; };
    const auto live = [&cutoff](int value) { return value >= cutoff; };
    cutoff = 5;
    CHECK(fixed(4) && !live(4));
    auto counter = make_counter(10);
    CHECK(counter() == 11);
    CHECK(counter() == 12);
    auto independent = counter;
    CHECK(independent() == 13 && counter() == 13);
    const auto add = [](auto left, auto right) { return left + right; };
    CHECK(add(2, 3) == 5 && add(2.0, 0.5) == 2.5);
    std::vector<int> data{1, 2, 3};
    std::transform(data.begin(), data.end(), data.begin(),
                   [](int value) { return value * value; });
    CHECK(data == std::vector<int>({1, 4, 9}));
    course::report();
}
