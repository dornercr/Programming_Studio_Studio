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

// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    const std::vector<int> input{3, 8};
    PlainImport plain;
    DoubleImport doubled;
    check(plain.run(input) == "3 8 ");
    check(doubled.run(input) == "6 16 scaled");
    std::cout << '[' << plain.run(input) << "]\n";
    std::cout << '[' << doubled.run(input) << "]\n";
    check(plain.run({0, 100}) == "0 100 ");
    check(doubled.run({100}) == "200 scaled");
    check(input == std::vector<int>({3, 8}));
    bool rejected = false;
    try { plain.run({3, -1}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { plain.run({}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    BrokenImport broken;
    rejected = false;
    try { broken.run({0}); }
    catch (const std::logic_error&) { rejected = true; }
    check(rejected);
    std::cout << "raw, empty, and step violations rejected\n";
    OffsetImport offset;
    check(offset.run({0, 100}) == "10 110 offset");
    check(offset.run(input) == "13 18 offset");
    rejected = false;
    try { offset.run({101}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && input == std::vector<int>({3, 8}));
    std::cout << '[' << offset.run(input) << "]\n";
}
