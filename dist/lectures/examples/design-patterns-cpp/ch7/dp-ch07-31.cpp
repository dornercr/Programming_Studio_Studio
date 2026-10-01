#include <iostream>
#include <stdexcept>
#include <string>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
class Writer {
public:
    virtual ~Writer() = default;
    virtual std::string field(
        const std::string& name, int n) const = 0;
};
class Plain final : public Writer {
public:
    std::string field(const std::string& name, int n) const override {
        return name + "=" + std::to_string(n);
    }
};
class Brackets final : public Writer {
public:
    std::string field(const std::string& name, int n) const override {
        return "[" + name + ":" + std::to_string(n) + "]";
    }
};
class Csv final : public Writer {
public:
    std::string field(const std::string& name, int n) const override {
        return name + "," + std::to_string(n);
    }
};
class Report {
protected:
    // Borrow the chosen writer.
    const Writer& writer_;
    static void validate(int n) {
        if (n < 0) {
            throw std::invalid_argument(
                "count");
        }
    } // End validation.
public:
    explicit Report(const Writer& w) : writer_(w) {}
    virtual ~Report() = default;
    virtual std::string render(int n) const = 0;
};
class CountReport final : public Report {
public:
    explicit CountReport(const Writer& w) : Report(w) {}
    std::string // Count rendering.
    render(int n) const override {
        // This layer chooses meaning.
        validate(n);
        return writer_.field("count", n);
    } // End count report.
};
class WarningReport final : public Report {
public:
    explicit WarningReport(const Writer& w) : Report(w) {}
    std::string
    render(int n) const override {
        validate(n);
        const std::string label = n > 5 ? "high" : "normal";
        return writer_.field(label, n);
    }
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
  Csv w;
  return WarningReport(w).render(5);
})() << '\n';
}
