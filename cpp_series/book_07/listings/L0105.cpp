#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    harbor::LamportClock a,b;
    auto send=a.tick();
    b.tick(); auto receive=b.receive(send);
    harbor::check(send<receive,"happens-before clock condition");
    auto reply=b.tick(); a.receive(reply);
    harbor::check(a.value()>reply,"causal merge");
    std::cout<<"Logical timestamps order message causality, not elapsed seconds.\n";
}
