#include "course_test.hpp"
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

struct MoveFailure final {};
struct Metrics { unsigned copies{}; unsigned moves{}; bool reject_move{}; };

template<bool Nonthrowing>
class Item {
    int value_{};
    Metrics* metrics_{};
public:
    Item(int value, Metrics& metrics) : value_(value), metrics_(&metrics) {}
    Item(const Item& other) : value_(other.value_), metrics_(other.metrics_) {
        ++metrics_->copies;
    }
    Item(Item&& other) noexcept(Nonthrowing) : metrics_(other.metrics_) {
        if constexpr (!Nonthrowing) {
            if (metrics_->reject_move) throw MoveFailure{};
        }
        value_ = std::exchange(other.value_, 0);
        ++metrics_->moves;
    }
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;
    int value() const noexcept { return value_; }
};

template<bool Nonthrowing>
void observe_relocation() {
    Metrics metrics;
    std::vector<Item<Nonthrowing>> values;
    values.reserve(3);
    values.emplace_back(11, metrics);
    values.emplace_back(22, metrics);
    values.emplace_back(33, metrics);
    const auto capacity = values.capacity();
    values.reserve(capacity + 8);
    CHECK(values.size() == 3);
    CHECK(values[0].value() == 11 && values[1].value() == 22
          && values[2].value() == 33);
    // Counts are observations, not portable fixture expectations.
    std::cout << "nothrow=" << Nonthrowing << " copies=" << metrics.copies
              << " moves=" << metrics.moves << '\n';
}
int main() {
    static_assert(std::is_nothrow_move_constructible_v<Item<true>>);
    static_assert(!std::is_nothrow_move_constructible_v<Item<false>>);
    observe_relocation<true>();
    observe_relocation<false>();
    Metrics metrics;
    Item<false> original{42, metrics};
    metrics.reject_move = true;
    CHECK(course::throws<MoveFailure>([&] { Item<false> rejected{std::move(original)}; }));
    CHECK(original.value() == 42); // This class checks before changing the source.
    metrics.reject_move = false;
    Item<false> accepted{std::move(original)};
    CHECK(accepted.value() == 42 && original.value() == 0);
    course::report();
}
