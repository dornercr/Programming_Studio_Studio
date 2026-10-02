#include <iostream>

class Quantity {
public:
    static constexpr int maximum{1000};
    bool set(int candidate) {
        if (candidate < 0 || candidate > maximum) return false;
        value_ = candidate;
        return true;
    }
    int value() const { return value_; }
private:
    int value_{};
};

int main() {
    Quantity quantity;
    if (quantity.value() != 0) return 1;
    if (!quantity.set(12) || quantity.value() != 12) return 2;
    if (quantity.set(-1) || quantity.value() != 12) return 3;
    if (quantity.set(1001) || quantity.value() != 12) return 4;
    const Quantity& observed = quantity;
    std::cout << "quantity=" << observed.value() << '\n';
    std::cout << "PASS\n";
}
