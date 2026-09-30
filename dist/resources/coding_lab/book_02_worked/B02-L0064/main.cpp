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

// Original book listing B02-L0064
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
