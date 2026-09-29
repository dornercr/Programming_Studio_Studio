#include "harbor/check.hpp"
#include "harbor/store.hpp"
#include <iostream>
int main() {
    harbor::Store api(":memory:"),worker(":memory:");
    for (int i=0;i<10;++i) {
        harbor::Request r{"job-"+std::to_string(i),i}; api.submit(r);
    }
    for (const auto& r:api.pending()) {
        auto result=worker.apply(r);
        harbor::check(worker.apply(r)==result,"duplicate effect result");
        api.complete(r,result);
    }
    harbor::check(api.pending().empty() && worker.receipt_count()==10,"end state");
    std::cout<<"Ten jobs completed with stable identities; process/network tests are separate.\n";
}
