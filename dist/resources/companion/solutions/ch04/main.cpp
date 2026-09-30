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

int main() {
    std::vector<std::unique_ptr<Cue>> originals;
    originals.push_back(std::make_unique<TextCue>("Start"));
    originals.push_back(std::make_unique<ListCue>(
        std::vector<std::string>{"Check", "Begin"}));
    std::vector<std::unique_ptr<Cue>> copies;
    // No concrete type is named here.
    for (const auto& cue : originals) {
        copies.push_back(cue->clone());
        check(copies.back()->text() ==
              cue->text());
    }
    copies[1]->rename("Inspect");
    check(originals[1]->text() == "list:Check;Begin;");
    check(copies[1]->text() == "list:Inspect;Begin;");
    check(copies[0].get() != originals[0].get());
    std::cout << originals[1]->text() << '\n';
    std::cout << copies[1]->text() << '\n';
    bool rejected = false;
    try { copies[1]->rename(""); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && copies[1]->text() == "list:Inspect;Begin;");
    rejected = false;
    try { ListCue empty(std::vector<std::string>{}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected);
    std::cout << "independent copies; invalid edits rejected\n";
    std::unique_ptr<Cue> box = std::make_unique<BoxCue>("Open");
    auto copy = box->clone();
    check(copy->text() == "box:Open");
    copy->rename("Close");
    check(box->text() == "box:Open");
    box.reset();
    check(copy->text() == "box:Close");
    rejected = false;
    try { copy->rename(""); }
    catch (const std::invalid_argument&) { rejected = true; }
    check(rejected && copy->text() == "box:Close");
    std::cout << "box:Close survives original destruction\n";
}
