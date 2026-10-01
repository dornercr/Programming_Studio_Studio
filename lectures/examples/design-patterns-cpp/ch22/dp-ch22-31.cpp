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
        lines.push_back("cable-m=" + std::to_string(cable.metres()));
    }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("visitor check failed"); }
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
  Labels l;
  Cable(3).accept(l);
  return l.lines.at(0);
})() << '\n';
}
