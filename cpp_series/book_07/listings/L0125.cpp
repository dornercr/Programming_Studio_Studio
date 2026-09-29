#include "harbor/check.hpp"
#include "harbor/models.hpp"
#include <iostream>
int main() {
    harbor::FencedRegister storage;
    harbor::check(storage.write(7,"old owner"),"initial term");
    harbor::check(storage.write(8,"new owner"),"new term");
    harbor::check(!storage.write(7,"late stale write"),"fence stale writer");
    harbor::check(storage.value()=="new owner","value preserved");
    std::cout<<"Storage rejected an old epoch; token issuance still requires authority.\n";
}
