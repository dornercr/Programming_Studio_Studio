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
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    Plain plain;
    Brackets brackets;
    CountReport cp(plain);
    CountReport cb(brackets);
    WarningReport wp(plain);
    WarningReport wb(brackets);
    check(cp.render(6) == "count=6");
    check(cb.render(6) == "[count:6]");
    check(wp.render(6) == "high=6");
    check(wb.render(6) == "[high:6]");
    check(wb.render(5) == "[normal:5]");
    check(cp.render(0) == "count=0");
    bool rejected = false;
    try { static_cast<void>(wp.render(-1)); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    Csv csv;
    CountReport cc(csv);
    WarningReport wc(csv);
    check(cc.render(6) == "count,6");
    check(wc.render(5) == "normal,5");
    check(wc.render(6) == "high,6");
    check(cc.render(0) == "count,0");
    std::cout << cc.render(6) << '\n' << wc.render(6) << '\n';
    std::cout << cp.render(6) << '\n' << cb.render(6) << '\n';
    std::cout << wp.render(6) << '\n' << wb.render(6) << '\n';
    std::cout << "negative count rejected\n";
}
