#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Listener {
public:
    virtual ~Listener() = default;
    virtual void update(int value) = 0;
};
class Feed {
    std::vector<std::weak_ptr<Listener>> listeners_;
    bool notifying_ = false;
    struct Reset {
        bool& flag;
        ~Reset() { /* defect: busy forever */ }
    };
public:
    Feed() = default;
    Feed(const Feed&) = delete;
    Feed& operator=(const Feed&) = delete;
    void subscribe(const std::shared_ptr<Listener>& listener) {
        if (notifying_) { throw std::logic_error("busy feed"); }
        if (!listener) { throw std::invalid_argument("null listener"); }
        for (const auto& weak : listeners_) {
            if (weak.lock() == listener) { return; }
        }
        listeners_.push_back(listener);
    }
    void unsubscribe(const std::shared_ptr<Listener>& listener) {
        if (notifying_) { throw std::logic_error("busy feed"); }
        listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(),
            [&](const std::weak_ptr<Listener>& weak) {
                const auto live = weak.lock();
                return !live || live == listener;
            }), listeners_.end());
    }
    void publish(int value) {
        if (value < 0 || value > 100) {
            throw std::out_of_range("reading range");
        }
        if (notifying_) { throw std::logic_error("nested publish"); }
        // Clear the flag on every exit.
        notifying_ = true;
        Reset reset{notifying_};
        for (const auto& weak : listeners_) {
            if (auto live = weak.lock()) {
                live->update(value);
            }
        }
    } // publish
};
class Total final : public Listener {
public:
    int sum = 0;
    std::function<void()> after;
    void update(int value) override {
        // Update happens before the hook.
        sum += value;
        // A throwing hook leaves the sum.
        if (after) { after(); }
    }
};
void check(bool okay) {
    if (!okay) { throw std::runtime_error("observer check failed"); }
}
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    Feed feed;
    auto first = std::make_shared<Total>();
    auto expired = std::make_shared<Total>();
    feed.subscribe(first);
    feed.subscribe(first);
    feed.subscribe(expired);
    expired.reset();
    feed.publish(4);
    check(first->sum == 4);
    bool nested = false;
    bool mutation = false;
    first->after = [&] {
        try { feed.publish(1); }
        catch (const std::logic_error&) { nested = true; }
        try { feed.subscribe(first); }
        catch (const std::logic_error&) { mutation = true; }
    };
    feed.publish(3);
    check(nested && mutation && first->sum == 7);
    std::cout << "sum: " << first->sum << '\n';
    first->after = [] { throw std::runtime_error("listener failed"); };
    bool callback_failed = false;
    try { feed.publish(2); }
    catch (const std::runtime_error&) { callback_failed = true; }
    check(callback_failed && first->sum == 9);
    first->after = {};
    feed.publish(1);
    check(first->sum == 10);
    bool invalid = false;
    try { feed.publish(-1); }
    catch (const std::out_of_range&) { invalid = true; }
    check(invalid && first->sum == 10);
    bool null_rejected = false;
    try { feed.subscribe({}); }
    catch (const std::invalid_argument&) { null_rejected = true; }
    check(null_rejected);
    std::cout << "after callback failure: " << first->sum << '\n';
    std::cout << "nested publish: rejected\n";

    bool removal_blocked = false;
    first->after = [&] {
        try { feed.unsubscribe(first); }
        catch (const std::logic_error&) { removal_blocked = true; }
    };
    feed.publish(1);
    check(removal_blocked && first->sum == 11);
    first->after = {};
    feed.unsubscribe(first);
    feed.unsubscribe(first);
    feed.publish(5);
    check(first->sum == 11);
    feed.subscribe(first);
    feed.publish(2);
    check(first->sum == 13);
    std::cout << "unsubscribe checks: passed\n";
}
