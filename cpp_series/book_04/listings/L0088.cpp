#include "check.hpp"
#include <array>
#include <mutex>
#include <stdexcept>
#include <thread>

class Account {
    mutable std::mutex mutex_;
    int balance_;
public:
    explicit Account(int balance) : balance_(balance) {
        if (balance < 0 || balance > 1000000) throw std::invalid_argument("balance");
    }
    int balance() const { std::lock_guard lock{mutex_}; return balance_; }
    friend bool transfer(Account& from, Account& to, int amount) {
        if (&from == &to || amount <= 0 || amount > 1000000) return false;
        std::scoped_lock lock{from.mutex_, to.mutex_};
        if (from.balance_ < amount || to.balance_ > 1000000 - amount) return false;
        from.balance_ -= amount;
        to.balance_ += amount;
        return true;
    }
};
int main() {
    Account a{10000}, b{10000};
    std::array<bool, 2> accepted{true, true};
    std::jthread first([&] {
        for (int i = 0; i < 1000; ++i) if (!transfer(a, b, 1)) accepted[0] = false;
    });
    std::jthread second([&] {
        for (int i = 0; i < 1000; ++i) if (!transfer(b, a, 1)) accepted[1] = false;
    });
    first.join(); second.join();
    CHECK(accepted[0] && accepted[1]);
    CHECK(a.balance() == 10000 && b.balance() == 10000);
    CHECK(!transfer(a, a, 1));
    CHECK(!transfer(a, b, 20000));
    CHECK(a.balance() + b.balance() == 20000);
    std::cout << "PASS\n";
}
