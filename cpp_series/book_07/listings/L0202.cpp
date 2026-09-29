#include "harbor/check.hpp"
#include "harbor/policies.hpp"
#include <iostream>
int main() {
    harbor::Breaker circuit(2,100);
    circuit.complete(*circuit.acquire(0),false,0);
    circuit.complete(*circuit.acquire(1),false,1);
    harbor::check(!circuit.acquire(100),"open cooldown");
    auto probe=circuit.acquire(101); harbor::check(probe.has_value(),"probe available");
    harbor::check(!circuit.acquire(101),"only one recovery probe");
    circuit.complete(*probe,true,102);
    harbor::check(circuit.state()==harbor::Breaker::State::closed,"closed after proof");
    std::cout<<"Two failures opened the circuit; one later probe established recovery.\n";
}
