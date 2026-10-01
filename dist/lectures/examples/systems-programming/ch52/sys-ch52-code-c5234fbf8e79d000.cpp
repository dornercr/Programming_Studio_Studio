#include <cassert>
#include <iostream>
#include <mutex>

class Gate {
    unsigned active_ = 0;
    const unsigned limit_;
    std::mutex mutex_;
public:
    explicit Gate(unsigned limit) : limit_(limit) {}
    bool admit() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(active_ == limit_) return false;
        ++active_; return true;
    }
    bool release() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(active_ == 0) return false;
        --active_; return true;
    }
};

int main() {
    Gate gate(2);
    assert(gate.admit() && gate.admit() && !gate.admit());
    assert(gate.release() && gate.admit());
    assert(gate.release() && gate.release() && !gate.release());
    Gate closed(0); assert(!closed.admit());
    std::cout << "limit=2 third=rejected released=reusable\n";
}
