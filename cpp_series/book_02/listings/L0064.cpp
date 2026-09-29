#include "course_test.hpp"
#include <forward_list>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3, 4, 5, 6};
    for (auto it = values.begin(); it != values.end();) {
        if (*it % 2 != 0) it = values.erase(it);
        else ++it;
    }
    CHECK(values == std::vector<int>({2, 4, 6}));
    const std::size_t saved_index{1};
    const int before = values[saved_index];
    values.reserve(values.capacity() + 100);
    CHECK(values[saved_index] == before);
    const std::forward_list<int> singly{2, 4, 6};
    CHECK(std::distance(singly.begin(), singly.end()) == 3);
    CHECK(std::distance(values.begin(), values.end()) == 3);
    auto it = values.begin();
    std::advance(it, 2);
    CHECK(*it == 6);
    course::report();
}
