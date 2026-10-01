#include <algorithm>
#include <iostream>
#include <memory>

class Estimate {
public:
    virtual ~Estimate() = default;
    // Contract: a and b are both in [0, 1000].
    virtual int value(int a, int b) const = 0;
};
class Mean final : public Estimate {
public:
    int value(int a, int b) const override { return (a + b) / 2; }
};
class Highest final : public Estimate {
public:
    int value(int a, int b) const override { return std::max(a, b); }
};
int main() {
    std::unique_ptr<Estimate> rule = std::make_unique<Mean>();
    std::cout << "mean=" << rule->value(1, 9) << '\n';
    rule = std::make_unique<Highest>(); // Replace the owned rule.
    std::cout << "highest=" << rule->value(1, 9) << '\n';
}
