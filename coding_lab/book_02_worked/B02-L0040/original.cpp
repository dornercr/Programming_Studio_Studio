#include "course_test.hpp"
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
