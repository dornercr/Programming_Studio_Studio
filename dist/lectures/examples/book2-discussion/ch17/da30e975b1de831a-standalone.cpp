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

// Original book listing B02-L0072
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

struct Job { int priority; int sequence; };

int main() {
    std::vector<Job> jobs{{2, 0}, {1, 1}, {2, 2}, {1, 3}};
    std::stable_sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.priority < b.priority;
    });
    CHECK(jobs[0].sequence == 1 && jobs[1].sequence == 3);
    CHECK(jobs[2].sequence == 0 && jobs[3].sequence == 2);
    const std::vector<int> data{1, 3, 5, 7};
    auto found = std::lower_bound(data.begin(), data.end(), 4);
    CHECK(found != data.end() && *found == 5);
    CHECK(std::find(data.begin(), data.end(), 2) == data.end());
    std::vector<int> squared;
    std::transform(data.begin(), data.end(), std::back_inserter(squared),
                   [](int value) { return value * value; });
    CHECK(squared == std::vector<int>({1, 9, 25, 49}));
    CHECK(std::accumulate(squared.begin(), squared.end(), 0LL) == 84);
    course::report();
}
