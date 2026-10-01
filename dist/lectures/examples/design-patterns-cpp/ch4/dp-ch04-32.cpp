#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class Cue {
public:
    virtual ~Cue() = default;
    virtual std::unique_ptr<Cue> clone() const = 0;
    virtual void rename(std::string name) = 0;
    virtual std::string text() const = 0;
};
class TextCue final : public Cue {
    std::string name_;
public:
    TextCue(const TextCue&) = default;
    explicit TextCue(std::string name) { rename(std::move(name)); }
    std::unique_ptr<Cue>
    clone() const override {
        return std::make_unique<TextCue>(*this);
    }
    void rename(std::string name) override {
        if (name.empty()) throw std::invalid_argument("name");
        name_ = std::move(name);
    }
    std::string text() const override { return "text:" + name_; }
};
class ListCue final : public Cue {
    std::vector<std::string> names_;
public:
    ListCue(const ListCue&) = default;
    explicit ListCue(std::vector<std::string> names)
        : names_(std::move(names)) {
        if (names_.empty()) throw std::invalid_argument("list");
        for (const auto& name : names_)
            if (name.empty()) throw std::invalid_argument("name");
    }
    std::unique_ptr<Cue>
    clone() const override {
        // Copy the vector and its strings.
        return
            std::make_unique<ListCue>(*this);
    }
    void rename(std::string name) override {
        if (name.empty())
            throw std::invalid_argument(
                "name");
        names_.front() = std::move(name);
    }
    std::string text() const override {
        std::string result = "list:";
        for (const auto& name : names_) result += name + ';';
        return result;
    }
};

class BoxCue final : public Cue {
    std::unique_ptr<std::string> name_;
public:
    explicit BoxCue(std::string name)
        : name_(std::make_unique<std::string>(std::move(name))) {
        if (name_->empty()) throw std::invalid_argument("name");
    }
    BoxCue(const BoxCue& other)
        : name_(std::make_unique<std::string>(*other.name_)) {}
    std::unique_ptr<Cue> clone() const override {
        return std::make_unique<BoxCue>(*this);
    }
    void rename(std::string name) override {
        if (name.empty()) throw std::invalid_argument("name");
        *name_ = std::move(name);
    }
    std::string text() const override { return "box:" + *name_; }
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
  return dpx_rejects<std::invalid_argument>([]{
    ListCue a(std::vector<std::string>{
    });
  });
})() << '\n';
}
