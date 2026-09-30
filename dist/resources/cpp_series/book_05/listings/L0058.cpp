#include "check.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
struct Tracked {
    inline static int alive = 0;
    Tracked() { ++alive; }
    ~Tracked() { --alive; }
    Tracked(const Tracked&) = delete;
    Tracked& operator=(const Tracked&) = delete;
};
struct PlannedFailure {};
void operation(bool fail) {
    auto owner = std::make_unique<Tracked>();
    std::vector<int> buffer(8, 3);
    CHECK(buffer.at(7) == 3);
    if (fail) throw PlannedFailure{};
}
int main() {
    operation(false); CHECK(Tracked::alive == 0);
    bool observed = false;
    try { operation(true); } catch (const PlannedFailure&) { observed = true; }
    CHECK(observed && Tracked::alive == 0);
    std::cout << "PASS\n";
}
