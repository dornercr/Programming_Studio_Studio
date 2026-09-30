#include "harbor/check.hpp"
#include "harbor/domain.hpp"
#include <iostream>
int main() {
    auto parsed=harbor::parse_request("job-17","250");
    auto* r=std::get_if<harbor::Request>(&parsed);
    harbor::check(r && harbor::evaluate(*r)==62500,"domain result");
    for (auto bad : {"-1","2 trailing","1000001",""})
        harbor::check(std::holds_alternative<harbor::Error>(
            harbor::parse_request("job-17",bad)),"invalid input escaped boundary");
    std::cout<<"A transport-independent contract rejects invalid work.\n";
}
