#include "course_test.hpp"
#include <array>
#include <numeric>
#include <vector>

struct Point { int x{}; int y{}; };
constexpr int square(int value) { return value * value; }

int main() {
    static_assert(square(7) == 49);
    std::vector<Point> original{{1, 2}, {3, 4}};
    auto copy = original;
    copy.front().x = 99;
    CHECK(original.front().x == 1);
    CHECK(copy.front().x == 99);
    const std::array<int, 4> values{1, 2, 3, 4};
    int loop_sum{};
    for (int value : values) loop_sum += value;
    const int algorithm_sum = std::accumulate(values.begin(), values.end(), 0);
    CHECK(loop_sum == 10 && algorithm_sum == loop_sum);
    const Point& borrowed = original.front();
    CHECK(borrowed.y == 2);
    course::report();
}
