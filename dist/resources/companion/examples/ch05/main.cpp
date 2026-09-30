#include <iostream>
#include <mutex>
#include <stdexcept>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
class TicketDispenser final {
    std::mutex gate_;
    int next_ = 1;
    TicketDispenser() = default;
    ~TicketDispenser() = default;
public:
    TicketDispenser(const TicketDispenser&) = delete;
    TicketDispenser& operator=(const TicketDispenser&) = delete;
    static TicketDispenser& instance() {
        // Initialize once on first use.
        static TicketDispenser dispenser;
        return dispenser;
    }
    int take() {
        // Guard every counter change.
        std::lock_guard<std::mutex>
            lock(gate_);
        if (next_ > 3)
            throw std::out_of_range(
                "exhausted");
        return next_++;
    }
};
int main() {
    auto& first = TicketDispenser::instance();
    auto& second = TicketDispenser::instance();
    check(&first == &second);
    const int a = first.take();
    const int b = second.take();
    const int c = first.take();
    check(a == 1 && b == 2 && c == 3);
    std::cout << "same instance=yes\n";
    std::cout << "tickets=" << a << ',' << b << ',' << c << '\n';
    bool rejected = false;
    try { second.take(); }
    catch (const std::out_of_range&) { rejected = true; }
    check(rejected);
    rejected = false;
    try { first.take(); }
    catch (const std::out_of_range&) { rejected = true; }
    check(rejected);
    std::cout << "exhausted remains exhausted\n";
}
