#include <iostream>
#include <map>
#include <utility>
int main() {
    // Same virtual page; two deliberately different address spaces.
    std::map<unsigned,unsigned> wrong{{1,7}};
    using Key = std::pair<unsigned,unsigned>; // address-space id, page
    const std::map<Key,unsigned> correct{{{10,1},7},{{20,1},9}};
    std::cout << "B-page-only=" << wrong.at(1) << '\n';
    std::cout << "B-context-key=" << correct.at({20,1}) << '\n';
}
