#include "harbor/check.hpp"
#include "harbor/policies.hpp"
#include <iostream>
int main() {
    harbor::Budget request{200};
    auto child_allowance=request.remaining(35);
    harbor::check(child_allowance==165,"parent budget");
    auto after_queue=request.remaining(140);
    harbor::check(after_queue==60,"queue residence must count");
    harbor::check(request.expired(200),"inclusive deadline");
    std::cout<<"200 ms total: 35 ms used, then 105 ms queued; only 60 ms remain.\n";
}
