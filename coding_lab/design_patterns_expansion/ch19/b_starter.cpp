#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

enum class Event { begin, finish, reset, cancel };
class State {
public:
    virtual ~State() = default;
    virtual const char* name() const = 0;
    virtual std::unique_ptr<State> next(Event event) const = 0;
};
class Idle final : public State {
public:
    const char* name() const override {
        return "idle";
    }
    std::unique_ptr<State> next(Event event) const override;
};
class Active final : public State {
public:
    const char* name() const override {
        return "active";
    }
    std::unique_ptr<State> next(Event event) const override;
};
class Done final : public State {
public:
    const char* name() const override {
        return "done";
    }
    std::unique_ptr<State> next(Event event) const override;
};
std::unique_ptr<State> Idle::next(Event event) const {
    if (event == Event::begin) { return std::make_unique<Active>(); }
    return nullptr;
}
std::unique_ptr<State> Active::next(Event event) const {
    if (event == Event::cancel) { return std::make_unique<Done>(); }
    // Finishing still moves to Done.
    if (event == Event::finish) {
        return std::make_unique<Done>();
    }
    return nullptr;
}
std::unique_ptr<State> Done::next(Event event) const {
    if (event == Event::reset) { return std::make_unique<Idle>(); }
    return nullptr;
}
class Session {
    std::unique_ptr<State> state_ = std::make_unique<Idle>();
public:
    Session() = default;
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
    std::string name() const { return state_->name(); }
    bool send(Event event) {
        // Finish the call before deletion.
        auto proposed = state_->next(event);
        if (!proposed) { return false; }
        state_ = std::move(proposed);
        return true;
    } // send
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("state check failed"); }
}
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    Session session;
    check(session.name() == "idle");
    check(!session.send(Event::finish));
    check(!session.send(Event::reset));
    std::cout << session.name() << '\n';
    check(session.send(Event::begin));
    check(session.name() == "active");
    check(!session.send(Event::begin));
    check(!session.send(Event::reset));
    std::cout << session.name() << '\n';
    check(session.send(Event::finish));
    check(session.name() == "done");
    check(!session.send(Event::begin));
    check(!session.send(Event::finish));
    std::cout << session.name() << '\n';
    check(session.send(Event::reset));
    check(session.name() == "idle");
    std::cout << session.name() << '\n';

    check(!session.send(Event::cancel));
    check(session.send(Event::begin));
    check(session.send(Event::cancel) && session.name() == "idle");
    check(session.send(Event::begin));
    check(session.send(Event::finish));
    check(!session.send(Event::cancel) && session.name() == "done");
    check(session.send(Event::reset));
    std::cout << "cancel: " << session.name() << '\n';
}
