#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
class Work {
public:
    virtual ~Work() = default;
    virtual int minutes() const = 0;
};
class Step final : public Work {
    int minutes_;
public:
    explicit Step(int n) : minutes_(n) {
        if (n < 0 || n > 10000) {
            throw std::invalid_argument("minutes");
        }
    }
    int minutes() const override { return minutes_; }
};
using Children = std::vector<std::unique_ptr<Work>>;
class Group final : public Work {
    Children children_;
public:
    // Take a finished list of children.
    explicit Group(Children children)
        : children_(std::move(children)) {
        for (const auto& child : children_) {
            if (!child) {
                throw std::invalid_argument(
                    "child");
            }
        }
    } // End group construction.
    int minutes() const override {
        // Add each child subtotal once.
        int total = 0;
        for (const auto& child : children_) {
            const int n = child->minutes();
            if (n > 10000 - total) {
                throw std::overflow_error(
                    "budget");
            }
            total += n;
        }
        return total;
    } // End recursive total.
};
int main() {
    Children small;
    small.push_back(std::make_unique<Step>(10));
    small.push_back(std::make_unique<Step>(20));
    Children large;
    large.push_back(std::make_unique<Group>(std::move(small)));
    large.push_back(std::make_unique<Step>(5));
    Group project(std::move(large));
    Group empty(Children{});
    check(project.minutes() == 35);
    check(empty.minutes() == 0);
    check(Step(10000).minutes() == 10000);
    bool bad_step = false;
    try { Step bad(-1); }
    catch (const std::invalid_argument&) { bad_step = true; }
    check(bad_step);
    bool bad_child = false;
    try {
        Children invalid;
        invalid.push_back(nullptr);
        Group bad(std::move(invalid));
    } catch (const std::invalid_argument&) { bad_child = true; }
    check(bad_child);
    bool too_large = false;
    try {
        Children huge;
        huge.push_back(std::make_unique<Step>(10000));
        huge.push_back(std::make_unique<Step>(1));
        Group over(std::move(huge));
        static_cast<void>(over.minutes());
    } catch (const std::overflow_error&) { too_large = true; }
    check(too_large);
    std::cout << "project=" << project.minutes() << " minutes\n";
    std::cout << "empty=" << empty.minutes() << " minutes\n";
    std::cout << "invalid child, duration, and total rejected\n";
}
