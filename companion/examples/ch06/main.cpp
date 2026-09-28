#include <cmath>
#include <iostream>
#include <stdexcept>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
class LegacyScale {
    int raw_;
public:
    explicit LegacyScale(int raw) : raw_(raw) {}
    int milligrams() const { return raw_; }
};
class Scale {
public:
    virtual ~Scale() = default;
    virtual double grams() const = 0;
};
class ScaleAdapter final : public Scale {
    const LegacyScale& device_;
public:
    // Borrow a device that must outlive us.
    explicit ScaleAdapter(
        const LegacyScale& device)
        : device_(device) {} // Borrow.
    double grams() const override {
        // Negative means device failure.
        const int raw = device_.milligrams();
        if (raw < 0) {
            throw std::runtime_error(
                "device");
        }
        return raw / 1000.0;
    } // End conversion.
};
// The application knows grams, not the old API.
bool light_enough(const Scale& scale) {
    return scale.grams() <= 2.0;
}
int main() {
    LegacyScale device(1250);
    ScaleAdapter scale(device);
    check(std::abs(scale.grams() - 1.25) < 1e-9);
    check(light_enough(scale));
    LegacyScale zero(0);
    ScaleAdapter empty(zero);
    check(empty.grams() == 0.0);
    LegacyScale limit(2000);
    ScaleAdapter boundary(limit);
    check(light_enough(boundary));
    LegacyScale heavy(2001);
    ScaleAdapter over(heavy);
    check(!light_enough(over));
    LegacyScale failed(-1);
    ScaleAdapter broken(failed);
    bool rejected = false;
    try { static_cast<void>(broken.grams()); }
    catch (const std::runtime_error&) { rejected = true; }
    check(rejected);
    std::cout << "grams=" << scale.grams() << '\n';
    std::cout << "light=" << light_enough(scale) << '\n';
    std::cout << "device error rejected\n";
}
