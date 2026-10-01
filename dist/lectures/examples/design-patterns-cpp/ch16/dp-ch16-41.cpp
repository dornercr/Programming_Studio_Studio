#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class Events {
public:
    virtual ~Events() = default;
    virtual void changed() = 0;
};
class NameField {
    Events& events_;
    std::string value_;
public:
    explicit NameField(Events& events) : events_(events) {}
    void set(std::string value) {
        // Equal input makes no event.
        if (value_ == value) { return; }
        value_ = std::move(value);
        events_.changed();
    } // field set
    bool filled() const { return !value_.empty(); }
};
class SaveButton {
    bool enabled_ = false;
public:
    void enable(bool value) { enabled_ = value; }
    bool enabled() const { return enabled_; }
};
class Form final : public Events {
    NameField name_;
    SaveButton save_;
    bool accepted_ = false;
    bool held_ = false;
    int saved_ = 0;
public:
    Form() : name_(*this) {}
    Form(const Form&) = delete;
    Form& operator=(const Form&) = delete;
    void changed() override {
        // Silent updates stop cycles.
        const bool ready = name_.filled();
        save_.enable(ready && accepted_ && !held_);
    } // coordination
    void name(std::string value) { name_.set(std::move(value)); }
    void accept(bool value) {
        accepted_ = value;
        changed();
    }
    void hold(bool value) { held_ = value; changed(); }
    bool enabled() const { return save_.enabled(); }
    bool submit() {
        if (!save_.enabled()) { return false; }
        ++saved_;
        return true;
    }
    int saved() const { return saved_; }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("form check failed"); }
}
int main() {
    Form form;
    check(!form.enabled() && !form.submit());
    form.name("Mina");
    check(!form.enabled());
    form.accept(true);
    check(form.enabled() && form.submit());
    check(form.saved() == 1);
    std::cout << "ready: " << form.enabled() << '\n';
    form.name("Mina");
    check(form.enabled());
    form.name("");
    check(!form.enabled() && !form.submit());
    check(form.saved() == 1);
    std::cout << "empty name: " << form.enabled() << '\n';
    form.name("Mina");
    form.accept(false);
    check(!form.enabled());
    std::cout << "saved: " << form.saved() << '\n';

    form.accept(true);
    check(form.enabled());
    form.hold(true);
    check(!form.enabled() && !form.submit() && form.saved() == 1);
    form.hold(false);
    check(form.enabled() && form.submit() && form.saved() == 2);
    form.name("");
    form.hold(true);
    form.hold(false);
    check(!form.enabled());
    std::cout << "hold checks: passed\n";
}
