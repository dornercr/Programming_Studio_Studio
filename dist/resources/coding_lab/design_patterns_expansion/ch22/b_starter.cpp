#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Bolts;
class Cable;
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(const Bolts& bolts) = 0;
    virtual void visit(const Cable& cable) = 0;
};
class Part {
public:
    virtual ~Part() = default;
    virtual void accept(Visitor& visitor) const = 0;
};
class Bolts final : public Part {
    int count_;
public:
    explicit Bolts(int count) : count_(count) {
        if (count < 0 || count > 100) {
            throw std::out_of_range("bolt count");
        }
    }
    int count() const { return count_; }
    void accept(Visitor& operation) const
        override {
        // Name this exact kind of part.
        const Bolts& self = *this;
        operation.visit(self);
    } // bolt accept
};
class Cable final : public Part {
    int metres_;
public:
    explicit Cable(int metres) : metres_(metres) {
        if (metres < 0 || metres > 100) {
            throw std::out_of_range("cable length");
        }
    }
    int metres() const { return metres_; }
    void accept(Visitor& visitor) const
        override {
        visitor.visit(*this);
    }
};
class Price final : public Visitor {
    int cents_ = 0;
    void add(int cents) {
        if (cents_ > 1000000 - cents) {
            throw std::overflow_error("price limit");
        }
        cents_ += cents;
    }
public:
    void visit(const Bolts& bolts) override {
        // Price this count of bolts.
        const int cost = bolts.count() * 5;
        add(cost);
    } // bolt price
    void visit(const Cable& cable) override {
        const int cost = cable.metres() * 40;
        add(cost);
    }
    int cents() const { return cents_; }
};
class Labels final : public Visitor {
public:
    std::vector<std::string> lines;
    void visit(const Bolts& bolts) override {
        lines.push_back("bolts=" + std::to_string(bolts.count()));
    }
    void visit(const Cable& cable) override {
        lines.push_back("bolts=" + std::to_string(cable.metres()));
    }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("visitor check failed"); }
}
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    std::vector<std::unique_ptr<Part>> parts;
    parts.push_back(std::make_unique<Bolts>(3));
    parts.push_back(std::make_unique<Cable>(2));
    Price price;
    for (const auto& part : parts) { part->accept(price); }
    check(price.cents() == 95);
    std::cout << "price cents: " << price.cents() << '\n';
    Price empty;
    check(empty.cents() == 0);
    Bolts zero(0);
    zero.accept(empty);
    check(empty.cents() == 0);
    bool invalid = false;
    try { Cable bad(-1); }
    catch (const std::out_of_range&) { invalid = true; }
    check(invalid);
    Cable reel(100);
    Price capped;
    for (int i = 0; i < 250; ++i) { reel.accept(capped); }
    check(capped.cents() == 1000000);
    bool overflow = false;
    try { reel.accept(capped); }
    catch (const std::overflow_error&) { overflow = true; }
    check(overflow && capped.cents() == 1000000);
    std::cout << "price limit: preserved\n";

    Labels labels;
    for (const auto& part : parts) { part->accept(labels); }
    check(labels.lines == std::vector<std::string>{"bolts=3", "cable-m=2"});
    check(price.cents() == 95);
    Labels none;
    check(none.lines.empty());
    zero.accept(none);
    check(none.lines == std::vector<std::string>{"bolts=0"});
    for (const auto& line : labels.lines) { std::cout << line << '\n'; }
}
