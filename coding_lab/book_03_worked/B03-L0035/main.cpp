// Shared test support from Book II, B02-L0110.
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

#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    const std::set<int> left{1, 2, 2, 4}, right{2, 3, 4};
    std::vector<int> intersection, united, difference;
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                          std::back_inserter(intersection));
    std::set_union(left.begin(), left.end(), right.begin(), right.end(),
                   std::back_inserter(united));
    std::set_difference(left.begin(), left.end(), right.begin(), right.end(),
                        std::back_inserter(difference));
    CHECK(intersection == std::vector<int>({2, 4}));
    CHECK(united == std::vector<int>({1, 2, 3, 4}));
    CHECK(difference == std::vector<int>({1}));
    std::map<std::string, int> ordered;
    std::unordered_map<std::string, int> hashed;
    for (const std::string word : {"read", "test", "read", "build"}) {
        ++ordered[word]; ++hashed[word]; // Insertion on absence is intended here.
    }
    CHECK(ordered.size() == 3 && ordered.at("read") == 2);
    for (const auto& [key, value] : ordered) CHECK(hashed.at(key) == value);
    CHECK(ordered.begin()->first == "build");
    course::report();
}
