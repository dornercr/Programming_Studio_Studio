#include <cassert>
#include <iostream>
#include <memory>
#include <string>



int main() {
    auto backing = std::make_shared<std::string>("cat");
    auto shared = backing;
    std::string snapshot = *backing;
    snapshot[0] = 'b';
    (*shared)[2] = 'r';
    assert(*backing == "car" && snapshot == "bat");
    backing.reset();
    assert(shared && *shared == "car");
    std::cout << "shared=" << *shared << " snapshot=" << snapshot << '\n';
}
