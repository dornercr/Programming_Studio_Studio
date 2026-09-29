#include "harbor/check.hpp"
#include "harbor/store.hpp"
#include <iostream>
int main() {
    harbor::Store gateway(":memory:"),worker(":memory:");
    harbor::Request r{"job-1",9}; gateway.submit(r);
    auto ignored_reply=worker.apply(r); (void)ignored_reply; // acknowledgment lost
    auto retried_reply=worker.apply(r);
    gateway.complete(r,retried_reply);
    harbor::check(worker.receipt_count()==1,"one durable effect record");
    harbor::check(gateway.lookup(r.key)->result==81,"reconciled response");
    std::cout<<"Lost acknowledgment caused a retry, not a second worker receipt.\n";
}
