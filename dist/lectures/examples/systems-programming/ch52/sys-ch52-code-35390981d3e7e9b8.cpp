#include <iostream>
#include <mutex>
#include <stdexcept>
class Gate {
    std::mutex mutex_;
    bool busy_ = false;
public:
    bool acquire() { std::lock_guard lock(mutex_); if (busy_) return false; busy_ = true; return true; }
    void release() { std::lock_guard lock(mutex_); busy_ = false; }
};
class Permit {
    Gate& gate_;
    bool owns_;
public:
    explicit Permit(Gate& gate) : gate_(gate), owns_(gate.acquire()) {}
    Permit(const Permit&) = delete;
    Permit& operator=(const Permit&) = delete;
    ~Permit() { if (owns_) gate_.release(); }
    bool owns() const { return owns_; }
};
int main() {
    Gate gate;
    try {
        Permit permit(gate);
        std::cout << "admitted=" << permit.owns() << '\n';
        throw std::runtime_error("request failed");
    } catch (const std::runtime_error&) { std::cout << "request failed\n"; }
    Permit next(gate);
    std::cout << "reusable=" << next.owns() << '\n';
}
