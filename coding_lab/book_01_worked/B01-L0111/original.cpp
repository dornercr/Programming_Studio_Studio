#include <iostream>
#include <stdexcept>

class Quantity {
public:
    Quantity() : Quantity(0) {}
    explicit Quantity(int value) : value_(value) {
        if (value < 0 || value > 1000) {
            throw std::invalid_argument("quantity out of range");
        }
    }
    int value() const { return value_; }
private:
    int value_;
};

class LifetimeProbe {
public:
    explicit LifetimeProbe(int& live) : live_(live) { ++live_; }
    ~LifetimeProbe() { --live_; }
    LifetimeProbe(const LifetimeProbe&) = delete;
    LifetimeProbe& operator=(const LifetimeProbe&) = delete;
private:
    int& live_;
};

int main() {
    Quantity empty;
    Quantity stock{7};
    if (empty.value() != 0 || stock.value() != 7) return 1;
    bool rejected = false;
    try { Quantity invalid{-1}; }
    catch (const std::invalid_argument&) { rejected = true; }
    if (!rejected) return 2;
    int live{};
    {
        LifetimeProbe first{live};
        { LifetimeProbe second{live}; if (live != 2) return 3; }
        if (live != 1) return 4;
    }
    if (live != 0) return 5;
    std::cout << "live=" << live << '\n';
    std::cout << "PASS\n";
}
