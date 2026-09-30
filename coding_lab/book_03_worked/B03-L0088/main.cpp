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
#include <cstddef>
#include <random>
#include <vector>
struct Record {
    int key;
    int arrival;
    bool operator==(const Record&) const = default;
};
void merge_sort(std::vector<Record>& values) {
    std::vector<Record> temporary(values.size());
    const auto sort = [&](auto&& self, std::size_t first, std::size_t last) -> void {
        if (last - first < 2) return;
        const auto middle = first + (last - first) / 2;
        self(self, first, middle); self(self, middle, last);
        auto left = first, right = middle, output = first;
        while (left < middle && right < last) {
            // Choose the left input on equal keys to preserve arrival order.
            if (values[right].key < values[left].key)
                temporary[output++] = values[right++];
            else temporary[output++] = values[left++];
        }
        while (left < middle) temporary[output++] = values[left++];
        while (right < last) temporary[output++] = values[right++];
        for (auto i = first; i < last; ++i) values[i] = temporary[i];
    };
    sort(sort, 0, values.size());
}
int main() {
    std::mt19937 random{616};
    for (int trial = 0; trial < 300; ++trial) {
        std::vector<Record> actual;
        for (int i = 0; i < trial % 129; ++i)
            actual.push_back({static_cast<int>(random() % 9) - 4, i});
        auto expected = actual;
        std::stable_sort(expected.begin(), expected.end(),
            [](const Record& a, const Record& b) { return a.key < b.key; });
        merge_sort(actual);
        CHECK(actual == expected);
        for (std::size_t i = 1; i < actual.size(); ++i) {
            CHECK(actual[i - 1].key <= actual[i].key);
            if (actual[i - 1].key == actual[i].key)
                CHECK(actual[i - 1].arrival < actual[i].arrival);
        }
    }
    course::report();
}
