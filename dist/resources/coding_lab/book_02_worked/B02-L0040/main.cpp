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

// Original book listing B02-L0040
#include <memory>
#include <vector>

class Filter {
public:
    virtual ~Filter() = default;
    virtual bool accepts(int value) const = 0;
};
class AtLeast final : public Filter {
    int minimum_;
public:
    explicit AtLeast(int minimum) : minimum_(minimum) {}
    bool accepts(int value) const override { return value >= minimum_; }
};
class Even final : public Filter {
public:
    bool accepts(int value) const override { return value % 2 == 0; }
};

int main() {
    std::vector<std::unique_ptr<Filter>> filters;
    filters.push_back(std::make_unique<AtLeast>(10));
    filters.push_back(std::make_unique<Even>());
    CHECK(filters[0]->accepts(11));
    CHECK(!filters[1]->accepts(11));
    CHECK(filters[0]->accepts(12) && filters[1]->accepts(12));
    filters.clear(); // Virtual destruction through owning base pointers.
    CHECK(filters.empty());
    course::report();
}
