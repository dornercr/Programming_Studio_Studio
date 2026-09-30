#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class Estimate {
public:
    virtual ~Estimate() = default;
    virtual int value(
        const std::vector<int>& data) const = 0;
};
class Average final : public Estimate {
public:
    int value(const std::vector<int>& data) const override {
        int sum = 0;
        for (int sample : data) sum += sample;
        return sum / static_cast<int>(data.size());
    }
};
class Highest final : public Estimate {
public:
    int value(const std::vector<int>& data) const override {
        return *std::max_element(data.begin(), data.end());
    }
};

class Middle final : public Estimate {
public:
    int value(const std::vector<int>& data) const override {
        auto sorted = data;
        std::sort(sorted.begin(), sorted.end());
        const auto middle = sorted.size() / 2;
        if (sorted.size() % 2 == 1) return sorted[middle];
        return (sorted[middle - 1] + sorted[middle]) / 2;
    }
};

class Monitor {
    std::unique_ptr<Estimate> rule_;
public:
    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;
    explicit Monitor(std::unique_ptr<Estimate> rule) {
        select(std::move(rule));
    }
    void select(
        std::unique_ptr<Estimate> rule) {
        // Keep the old rule on failure.
        if (!rule)
            throw std::invalid_argument(
                "rule");
        rule_ = std::move(rule);
    }
    int read(const std::vector<int>& data) const {
        if (data.empty() || data.size() > 1000)
            throw std::invalid_argument("size");
        for (int sample : data) {
            if (sample < 0 || sample > 1000)
                throw std::invalid_argument(
                    "sample");
        }
        // Validate before delegation.
        return rule_->value(data);
    }
};
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    const std::vector<int> data{1, 2, 9};
    Monitor monitor(std::make_unique<Average>());
    check(monitor.read(data) == 4);
    std::cout << "average=" << monitor.read(data) << '\n';
    monitor.select(std::make_unique<Highest>());
    check(monitor.read(data) == 9);
    std::cout << "highest=" << monitor.read(data) << '\n';
    check(monitor.read({0}) == 0);
    check(monitor.read({1000}) == 1000);
    bool rejected = false;
    try { monitor.read({}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { monitor.read({-1}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { monitor.select(nullptr); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && monitor.read(data) == 9);
    std::cout << "empty, range, and null rejected\n";
    monitor.select(std::make_unique<Middle>());
    check(monitor.read({9, 1, 2}) == 2);
    check(monitor.read({9, 1, 2, 4}) == 3);
    check(monitor.read({0}) == 0);
    check(monitor.read({1000, 1000}) == 1000);
    check(data == std::vector<int>({1, 2, 9}));
    std::cout << "middle odd=2 even=3; input unchanged\n";
}
