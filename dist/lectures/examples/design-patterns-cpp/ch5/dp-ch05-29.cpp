#include <iostream>
#include <mutex>
#include <stdexcept>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class TicketSource {
public:
    virtual ~TicketSource() = default;
    virtual int take() = 0;
};
class LocalTickets final : public TicketSource {
    std::mutex gate_;
    int next_ = 1;
public:
    int take() override {
        std::lock_guard<std::mutex> lock(gate_);
        if (next_ > 3) throw std::out_of_range("exhausted");
        return next_++;
    }
};
class TicketDispenser final : public TicketSource {
    LocalTickets source_;
    TicketDispenser() = default;
    ~TicketDispenser() override = default;
public:
    TicketDispenser(const TicketDispenser&) = delete;
    TicketDispenser& operator=(const TicketDispenser&) = delete;
    static TicketDispenser& instance() {
        static TicketDispenser dispenser;
        return dispenser;
    }
    int take() override { return source_.take(); }
};
int admit(TicketSource& source) { return source.take(); }
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  LocalTickets a;
  a.take();
  return admit(a);
})() << '\n';
}
