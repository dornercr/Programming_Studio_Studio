#include <iostream>
enum class State { Ready, Running, Waiting };
int main() {
    State a = State::Waiting;
    int cpu_owner = 2; // B owns this modeled single CPU.
    auto dispatch_a = [&] {
        if (a != State::Ready || cpu_owner != 0) return false;
        a = State::Running; cpu_owner = 1; return true;
    };
    std::cout << std::boolalpha << "waiting-dispatch=" << dispatch_a() << '\n';
    const bool woke = a == State::Waiting;
    if (woke) a = State::Ready;
    std::cout << "woke=" << woke << " busy-dispatch=" << dispatch_a() << '\n';
    cpu_owner = 0;
    const bool dispatched = dispatch_a();
    std::cout << "idle-dispatch=" << dispatched
              << " invariant=" << (a == State::Running && cpu_owner == 1) << '\n';
}
