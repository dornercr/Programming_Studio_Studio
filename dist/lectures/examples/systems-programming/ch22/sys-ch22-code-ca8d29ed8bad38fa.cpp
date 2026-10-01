#include <cassert>
#include <iostream>
#include <map>
#include <memory>

using Space = std::map<unsigned,std::shared_ptr<int>>;

int main() {
    Space a{{0x1000,std::make_shared<int>(7)}};
    Space b{{0x1000,std::make_shared<int>(9)}};
    const auto shared = std::make_shared<int>(2);
    a[0x2000] = shared; b[0x2000] = shared;
    *a.at(0x1000) = 11;
    *a.at(0x2000) = 5;
    assert(*b.at(0x1000) == 9 && *b.at(0x2000) == 5);
    assert(a.at(0x2000) == b.at(0x2000));
    std::cout << "private=" << *a.at(0x1000) << ',' << *b.at(0x1000)
              << " shared=" << *b.at(0x2000) << '\n';
}
