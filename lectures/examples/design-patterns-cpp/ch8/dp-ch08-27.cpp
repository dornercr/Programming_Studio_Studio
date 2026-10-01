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
    virtual std::size_t steps() const = 0;
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
    std::size_t steps() const override { return 1; }
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
    }
    std::size_t steps() const override {
        std::size_t total = 0;
        for (const auto& child : children_) total += child->steps();
        return total;
    } // End recursive total. // End group construction.
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
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  Children c;
  c.push_back(std::make_unique<Step>(7));
  c.push_back(std::make_unique<Step>(8));
  return Group(std::move(c)).minutes();
})() << '\n';
}
