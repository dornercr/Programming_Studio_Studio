#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    harbor::Saga saga;
    saga.event("e1","reserve"); saga.event("e2","charge");
    saga.event("e3","cancel"); saga.event("e4","compensated");
    harbor::check(!saga.event("e4","compensated"),"duplicate compensation acknowledgment");
    harbor::check(saga.state()==harbor::Saga::State::cancelled,"terminal state");
    harbor::rejects([&]{saga.event("e5","finish");},"late forward success");
    std::cout<<"Compensation is an explicit transition, not time running backward.\n";
}
