#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class Import {
protected:
    virtual int convert(int raw) const = 0;
    virtual std::string suffix() const { return ""; }
public:
    virtual ~Import() = default;
    std::string run(const std::vector<int>& input) const {
        if (input.empty())
            throw std::invalid_argument("empty");
        for (int raw : input) {
            if (raw < 0 || raw > 100)
                throw std::invalid_argument("raw");
        }
        // All input passed validation.
        std::ostringstream result;
        result.exceptions(std::ios::badbit |
                          std::ios::failbit);
        for (int raw : input) {
            const int value = convert(raw);
            if (value < 0 || value > 200)
                throw std::logic_error(
                    "converted");
            result << value << ' ';
        }
        return result.str() + suffix();
    }
};
class PlainImport final : public Import {
    // The required step keeps the value.
    int convert(int raw) const override {
        return raw;
    }
};
class DoubleImport final : public Import {
    // Double the input unit.
    int convert(int raw) const override {
        return raw * 2;
    }
    std::string suffix() const override {
        return "scaled";
    }
};
class BrokenImport final : public Import {
    int convert(int raw) const override { return raw + 201; }
};

class OffsetImport final : public Import {
    int convert(int raw) const override { return raw + 10; }
    std::string suffix() const override { return "offset"; }
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
  std::vector<int> v{
    2,4
  };
  OffsetImport{
  }.run(v);
  return v.front();
})() << '\n';
}
