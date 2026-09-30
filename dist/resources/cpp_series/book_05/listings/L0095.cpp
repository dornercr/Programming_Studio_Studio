#include "check.hpp"
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>
struct Entity { std::uint32_t id; bool active; int value; };
long long direct_total(std::span<const Entity> entities) {
    if (entities.size()>100000) throw std::invalid_argument("count");
    long long total=0;
    for (const auto& entity:entities) {
        if (entity.value < -1000000 || entity.value > 1000000) throw std::invalid_argument("value");
        if (entity.active) total+=entity.value;
    }
    return total;
}
class ActiveSnapshot {
    std::vector<int> values_;
public:
    explicit ActiveSnapshot(std::span<const Entity> entities) {
        if (entities.size()>100000) throw std::invalid_argument("count");
        values_.reserve(entities.size());
        for (const auto& entity:entities) {
            if (entity.value < -1000000 || entity.value > 1000000) throw std::invalid_argument("value");
            if (entity.active) values_.push_back(entity.value);
        }
    }
    long long total() const { long long sum=0; for (int value:values_) sum+=value; return sum; }
    std::size_t size() const { return values_.size(); }
};
int main() {
    std::vector<Entity> entities{{1,true,8},{2,false,100},{3,true,-3}};
    const ActiveSnapshot snapshot{entities};
    CHECK(snapshot.size()==2 && snapshot.total()==5 && direct_total(entities)==5);
    entities[1].active=true;
    CHECK(snapshot.total()==5); // Explicit snapshot semantics.
    const ActiveSnapshot updated{entities};
    CHECK(updated.total()==105 && direct_total(entities)==105);
    std::cout << "PASS\n";
}
