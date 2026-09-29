#include "check.hpp"
#include "accumulator.hpp"
#include <stdexcept>
#include <utility>
int main() {
    harbor::Accumulator first; first.add(7); first.add(-2);
    harbor::Accumulator second=std::move(first);
    CHECK(second.total()==5 && second.count()==2);
    bool rejected=false;
    try { (void)first.total(); } catch (const std::logic_error&) { rejected=true; }
    CHECK(rejected);
    first=harbor::Accumulator{}; first.add(3);
    CHECK(first.total()==3);
    std::cout << "PASS\n";
}
