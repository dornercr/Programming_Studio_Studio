#include <iostream>
#include <stdexcept>

class Probe {
public:
    explicit Probe(int& live) : live_(live) { ++live_; }
    ~Probe() { --live_; }
    Probe(const Probe&) = delete;
    Probe& operator=(const Probe&) = delete;
private:
    int& live_;
};

class FailsAfterMember {
public:
    explicit FailsAfterMember(int& live) : member_(live) {
        throw std::runtime_error("intentional construction rejection");
    }
private:
    Probe member_; // Completed member is cleaned up if the enclosing body throws.
};

int main() {
    int live{};
    bool caught{};
    try {
        Probe first{live};
        if (live != 1) return 1;
        throw std::runtime_error("intentional scope exit");
    } catch (const std::runtime_error&) {
        caught = true;
    }
    if (!caught || live != 0) return 2;

    caught = false;
    try {
        FailsAfterMember rejected{live};
    } catch (const std::runtime_error&) {
        caught = true;
    }
    if (!caught || live != 0) return 3;
    std::cout << "ordinary unwind and failed enclosing construction PASS\n";
}
