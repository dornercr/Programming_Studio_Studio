#include "course_test.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <optional>
#include <vector>

void validate(const std::vector<int>& coins, int amount) {
    if (amount < 0 || amount > 500) throw std::invalid_argument("amount 0..500");
    for (int coin : coins) if (coin <= 0) throw std::invalid_argument("positive coins required");
}
int memoized_count(const std::vector<int>& coins, int amount) {
    validate(coins, amount);
    std::vector<int> memo(static_cast<std::size_t>(amount) + 1, -2);
    memo[0] = 0;
    std::function<int(int)> solve = [&](int remaining) {
        int& result = memo[static_cast<std::size_t>(remaining)];
        if (result != -2) return result;
        result = -1;
        for (int coin : coins) {
            if (coin > remaining) continue;
            const int previous = solve(remaining - coin);
            if (previous != -1 && (result == -1 || previous + 1 < result)) result = previous + 1;
        }
        return result;
    };
    return solve(amount);
}
struct Solution { int count; std::vector<int> coins; };
std::optional<Solution> tabulated(const std::vector<int>& coins, int amount) {
    validate(coins, amount);
    std::vector<int> best(static_cast<std::size_t>(amount) + 1, amount + 1);
    std::vector<int> chosen(best.size(), -1);
    best[0] = 0;
    for (int value = 1; value <= amount; ++value) {
        for (int coin : coins) {
            if (coin <= value && best[static_cast<std::size_t>(value - coin)] + 1 <
                                 best[static_cast<std::size_t>(value)]) {
                best[static_cast<std::size_t>(value)] = best[static_cast<std::size_t>(value - coin)] + 1;
                chosen[static_cast<std::size_t>(value)] = coin;
            }
        }
    }
    if (best.back() > amount) return std::nullopt;
    Solution result{best.back(), {}};
    for (int value = amount; value > 0;) {
        const int coin = chosen[static_cast<std::size_t>(value)];
        result.coins.push_back(coin); value -= coin;
    }
    return result;
}

int main() {
    for (const auto& coins : {std::vector<int>{1, 3, 4}, {2, 4}, {3, 5, 7}, {}}) {
        for (int amount = 0; amount <= 100; ++amount) {
            const int top_down = memoized_count(coins, amount);
            const auto bottom_up = tabulated(coins, amount);
            CHECK(bottom_up.has_value() == (top_down != -1));
            if (bottom_up) {
                CHECK(bottom_up->count == top_down);
                CHECK(bottom_up->coins.size() == static_cast<std::size_t>(top_down));
                CHECK(std::accumulate(bottom_up->coins.begin(), bottom_up->coins.end(), 0) == amount);
            }
        }
    }
    CHECK(tabulated({1, 3, 4}, 6)->count == 2);
    CHECK(!tabulated({2}, 3));
    CHECK(course::throws<std::invalid_argument>([] { (void)tabulated({0, 1}, 2); }));
    course::report();
}
