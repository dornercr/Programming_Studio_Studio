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
int main() {
    const Plan intro = introduction();
    check(intro.title() == "Introduction" && intro.total() == 50);
    std::cout << intro.title() << ": " << intro.total() << " minutes\n";
    PlanBuilder builder;
    builder.named("Practice").add(30).add(30);
    const Plan first = builder.build();
    builder.add(30);
    const Plan second = builder.build();
    check(first.total() == 60 && second.total() == 90);
    std::cout << "snapshots=" << first.total() << ',' << second.total() << '\n';
    bool rejected = false;
    try { PlanBuilder{}.build(); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { builder.add(0); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && builder.build().total() == 90);
    rejected = false;
    try { builder.add(1); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { PlanBuilder{}.named("Long").add(60).add(60).build(); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    std::cout << "incomplete, invalid, full, and long rejected\n";
    builder.reset();
    rejected = false;
    try { builder.build(); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    builder.add(10);
    rejected = false;
    try { builder.build(); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    builder.reset();
    const Plan fresh = builder.named("Fresh").add(10).build();
    check(fresh.title() == "Fresh" && fresh.total() == 10);
    check(first.total() == 60 && second.total() == 90);
    builder.reset();
    builder.reset();
    std::cout << "reset keeps snapshots; fresh=10\n";
}
