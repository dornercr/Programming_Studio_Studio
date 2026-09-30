#include "harbor/check.hpp"
#include <iostream>
struct Lifecycle {
    bool initialized=false, draining=false;
    unsigned in_flight=0;
    bool ready() const { return initialized && !draining; }
    bool admit() { if (!ready()) return false; ++in_flight; return true; }
    void finish() { if (!in_flight) throw std::logic_error("unmatched completion"); --in_flight; }
    bool stopped() const { return draining && in_flight==0; }
};
int main() {
    Lifecycle l; harbor::check(!l.admit(),"no admission before initialization");
    l.initialized=true; harbor::check(l.admit(),"ready");
    l.draining=true; harbor::check(!l.admit() && !l.stopped(),"stop admission first");
    l.finish(); harbor::check(l.stopped(),"drain completion");
    std::cout<<"Readiness stopped before the last admitted request completed.\n";
}
