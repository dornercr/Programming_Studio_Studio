#include "harbor/check.hpp"
#include "harbor/store.hpp"
#include <iostream>
int main() {
    harbor::Store db(":memory:");
    harbor::Request r{"job-1",12};
    harbor::check(db.submit(r)==harbor::Store::Submit::accepted,"insert");
    harbor::check(db.submit(r)==harbor::Store::Submit::duplicate,"duplicate");
    harbor::check(db.submit({"job-1",13})==harbor::Store::Submit::conflict,"changed payload");
    harbor::check(db.pending().size()==1,"one committed intent");
    std::cout<<"Unique request identity and payload comparison share a transaction.\n";
}
