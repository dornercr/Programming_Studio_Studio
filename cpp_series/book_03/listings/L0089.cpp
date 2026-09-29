#include "course_test.hpp"
#include <algorithm>
#include <queue>
#include <stdexcept>
#include <vector>
void validate(const std::vector<int>& coins, int target) {
    if (target < 0 || target > 100) throw std::invalid_argument("target");
    for (int coin : coins) if (coin <= 0 || coin > 100)
        throw std::invalid_argument("denomination");
}
int tabulate(const std::vector<int>& coins, int target) {
    validate(coins, target);
    std::vector<int> best(static_cast<std::size_t>(target) + 1, 101);
    best[0] = 0;
    for (int amount = 1; amount <= target; ++amount)
        for (int coin : coins) if (coin <= amount)
            best[amount] = std::min(best[amount], best[amount - coin] + 1);
    return best[target] == 101 ? -1 : best[target];
}
int breadth_first(const std::vector<int>& coins, int target) {
    validate(coins, target);
    std::vector<int> distance(static_cast<std::size_t>(target) + 1, -1);
    std::queue<int> pending; pending.push(0); distance[0] = 0;
    while (!pending.empty()) {
        const int amount = pending.front(); pending.pop();
        for (int coin : coins) {
            if (coin > target - amount) continue;
            const int next = amount + coin;
            if (distance[next] != -1) continue;
            distance[next] = distance[amount] + 1; pending.push(next);
        }
    }
    return distance[target];
}
int main() {
    for (unsigned mask = 0; mask < 64; ++mask) {
        std::vector<int> coins;
        for (int coin = 1; coin <= 6; ++coin)
            if ((mask & (1U << (coin - 1))) != 0) coins.push_back(coin);
        for (int target = 0; target <= 40; ++target)
            CHECK(tabulate(coins, target) == breadth_first(coins, target));
    }
    CHECK(tabulate({1, 3, 4}, 6) == 2);
    CHECK(breadth_first({2, 4}, 3) == -1);
    CHECK(course::throws<std::invalid_argument>([] { tabulate({0}, 3); }));
    CHECK(course::throws<std::invalid_argument>([] { breadth_first({1}, 101); }));
    course::report();
}
