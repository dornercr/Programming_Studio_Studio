#include "check.hpp"
#include "pool.hpp"
#include <future>
#include <stdexcept>
#include <vector>
int main() {
    TaskPool pool{3,4};
    std::vector<std::future<long long>> results;
    for (int value=0;value<20;++value)
        results.push_back(pool.submit([value] { return static_cast<long long>(value)*value; }));
    for (int value=0;value<20;++value) CHECK(results[static_cast<std::size_t>(value)].get()==value*value);
    auto failed=pool.submit([]()->long long { throw std::runtime_error("planned task failure"); });
    bool propagated=false;
    try { (void)failed.get(); } catch (const std::runtime_error&) { propagated=true; }
    CHECK(propagated);
    pool.shutdown();
    bool closed=false;
    try { (void)pool.submit([] { return 1LL; }); } catch (const std::runtime_error&) { closed=true; }
    CHECK(closed);
    std::cout << "PASS\n";
}
