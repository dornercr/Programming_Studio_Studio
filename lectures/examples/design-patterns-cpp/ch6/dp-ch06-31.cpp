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
    double offset_;
public:
    // Borrow a device that must outlive us.
    explicit ScaleAdapter(
        const LegacyScale& device, double offset = 0.0)
        : device_(device), offset_(offset) {
        if (!std::isfinite(offset) || offset < 0.0) {
            throw std::invalid_argument("offset");
        }
    }
    double grams() const override {
        // Negative means device failure.
        const int raw = device_.milligrams();
        if (raw < 0) {
            throw std::runtime_error(
                "device");
        }
        const double value = raw / 1000.0 - offset_;
        if (value < 0.0) throw std::runtime_error("below zero");
        return value;
    }
};
// The application knows grams, not the old API.
bool light_enough(const Scale& scale) {
    return scale.grams() <= 2.0;
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
  LegacyScale d(500);
  ScaleAdapter a(d,0.5);
  return a.grams()==0.0;
})() << '\n';
}
