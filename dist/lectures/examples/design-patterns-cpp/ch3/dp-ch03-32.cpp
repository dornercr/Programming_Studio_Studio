#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class PlanBuilder;
class Plan {
    friend class PlanBuilder;
    std::string title_;
    std::vector<int> minutes_;
    Plan(std::string title, std::vector<int> minutes)
        : title_(std::move(title)), minutes_(std::move(minutes)) {}
public:
    Plan(const Plan&) = default;
    const std::string& title() const { return title_; }
    int total() const {
        return std::accumulate(minutes_.begin(), minutes_.end(), 0);
    }
};
class PlanBuilder {
    std::string title_;
    std::vector<int> minutes_;
public:
    PlanBuilder& named(std::string title) {
        if (title.empty()) throw std::invalid_argument("title");
        title_ = std::move(title);
        return *this;
    }
    PlanBuilder& add(int minutes) {
        if (minutes < 1 || minutes > 60 || minutes_.size() == 3)
            throw std::invalid_argument("session");
        minutes_.push_back(minutes);
        return *this;
    }
    void reset() {
        title_.clear();
        minutes_.clear();
    }
    Plan build() const {
        // A draft is not yet a Plan.
        if (title_.empty() ||
            minutes_.empty())
            throw std::logic_error(
                "incomplete");
        const int total = std::accumulate(
            minutes_.begin(),
            minutes_.end(), 0);
        if (total > 90)
            throw std::logic_error(
                "too long");
        return Plan(title_, minutes_);
    }
};
Plan introduction() {
    // This recipe is the director.
    PlanBuilder builder;
    builder.named("Introduction");
    builder.add(20).add(30);
    return builder.build();
}
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  return dpx_rejects<std::invalid_argument>([]{
    PlanBuilder{
    }.add(0);
  });
})() << '\n';
}
