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

// Original book listing B02-L0097
#include <initializer_list>
#include <utility>
#include <vector>

struct CopyFailure final {};
struct CopyBudget {
    int remaining{-1}; // Negative means copying is unrestricted.
    void consume() {
        if (remaining == 0) throw CopyFailure{};
        if (remaining > 0) --remaining;
    }
};
struct Cell {
    int value{};
    CopyBudget* budget{}; // Test policy must outlive every cell.
    Cell(int v, CopyBudget& b) : value(v), budget(&b) {}
    Cell(const Cell& other) : value(other.value), budget(other.budget) {
        budget->consume();
    }
    Cell(Cell&&) noexcept = default;
    Cell& operator=(const Cell&) = default;
    Cell& operator=(Cell&&) noexcept = default;
};
class Sequence {
    std::vector<Cell> cells_;
public:
    Sequence(CopyBudget& budget, std::initializer_list<int> values) {
        cells_.reserve(values.size());
        for (const int value : values) cells_.emplace_back(value, budget);
    }
    Sequence(const Sequence&) = default;
    Sequence& operator=(const Sequence& other) {
        Sequence candidate{other};
        cells_.swap(candidate.cells_); // Default allocator: nonthrowing commit.
        return *this;
    }
    std::vector<int> snapshot() const {
        std::vector<int> result;
        for (const Cell& cell : cells_) result.push_back(cell.value);
        return result;
    }
    void replace_first(int value) { cells_.at(0).value = value; }
};

int main() {
    CopyBudget source_budget, destination_budget;
    Sequence source{source_budget, {10, 20, 30}};
    const auto source_before = source.snapshot();
    for (int failure_point = 0; failure_point < 3; ++failure_point) {
        Sequence destination{destination_budget, {7, 8}};
        const auto before = destination.snapshot();
        source_budget.remaining = failure_point;
        CHECK(course::throws<CopyFailure>([&] { destination = source; }));
        CHECK(destination.snapshot() == before);
        CHECK(source.snapshot() == source_before);
        source_budget.remaining = -1;
        destination = source;
        CHECK(destination.snapshot() == source_before);
        destination.replace_first(99);
        CHECK(source.snapshot() == source_before);
        CHECK(destination.snapshot().front() == 99);
    }
    Sequence copy{source};
    copy.replace_first(-1);
    CHECK(source.snapshot() == source_before);
    const Sequence& alias = copy;
    const auto before_self = copy.snapshot();
    copy = alias;
    CHECK(copy.snapshot() == before_self);
    Sequence empty{source_budget, {}};
    copy = empty;
    CHECK(copy.snapshot().empty());
    Sequence empty_copy{empty};
    CHECK(empty_copy.snapshot().empty());
    course::report();
}
