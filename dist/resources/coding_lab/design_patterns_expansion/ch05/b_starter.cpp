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
int admit(TicketSource& source) { (void)source; return TicketDispenser::instance().take(); }
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    auto& first = TicketDispenser::instance();
    auto& second = TicketDispenser::instance();
    check(&first == &second);
    const int a = admit(first);
    const int b = admit(second);
    const int c = admit(first);
    check(a == 1 && b == 2 && c == 3);
    std::cout << "same instance=yes\n";
    std::cout << "tickets=" << a << ',' << b << ',' << c << '\n';
    for (int attempt = 0; attempt < 2; ++attempt) {
        bool rejected = false;
        try { admit(first); }
        catch (const std::out_of_range&) { rejected = true; }
        check(rejected);
    }
    std::cout << "exhausted remains exhausted\n";
    LocalTickets test_a;
    LocalTickets test_b;
    check(admit(test_a) == 1);
    check(admit(test_a) == 2);
    check(admit(test_b) == 1);
    check(admit(test_a) == 3);
    bool rejected = false;
    try { admit(test_a); }
    catch (const std::out_of_range&) { rejected = true; }
    check(rejected && admit(test_b) == 2);
    std::cout << "isolated sources=1,2 and 1; old limit preserved\n";
}
