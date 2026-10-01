#include <algorithm>
#include <array>
#include <iostream>
enum class Owner { free, producer, ready, worker };
int main() {
    std::array<Owner, 2> slots{Owner::free, Owner::free};
    const auto transfer = [&](std::size_t slot, Owner from, Owner to) {
        if (slots.at(slot) != from) return false;
        slots[slot] = to;
        return true;
    };
    if (!transfer(0, Owner::free, Owner::producer)
        || !transfer(0, Owner::producer, Owner::ready)
        || !transfer(0, Owner::ready, Owner::worker)
        || !transfer(0, Owner::worker, Owner::free)) return 1;
    const bool duplicate = transfer(0, Owner::worker, Owner::free);
    std::cout << "free slots=" << std::count(slots.begin(), slots.end(), Owner::free) << '\n';
    std::cout << "duplicate rejected=" << !duplicate << '\n';
}
