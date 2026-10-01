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
        ~Reset() { flag = false; }
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
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  Feed f;
  auto a=std::make_shared<Total>();
  f.subscribe(a);
  f.subscribe(a);
  f.publish(6);
  return a->sum;
})() << '\n';
}
