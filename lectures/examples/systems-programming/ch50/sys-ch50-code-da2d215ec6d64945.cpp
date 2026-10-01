#include <cassert>
#include <iostream>
#include <mutex>
#include <future>

struct Account { int balance = 100; std::mutex mutex; };
bool transfer(Account& from, Account& to, int amount) {
    if(amount < 0) return false;
    if(&from == &to) return true;
    std::scoped_lock lock(from.mutex,to.mutex);
    if(from.balance < amount) return false;
    from.balance -= amount; to.balance += amount;
    return true;
}

int main() {
    Account first, second;
    const auto worker = [&](bool forward) {
        for(int i=0;i<100;++i)
            assert(forward ? transfer(first,second,1) : transfer(second,first,1));
    };
    auto a = std::async(std::launch::async,worker,true);
    auto b = std::async(std::launch::async,worker,false);
    a.get(); b.get();
    assert(first.balance == 100 && second.balance == 100);
    assert(transfer(first,first,10) && first.balance == 100);
    assert(!transfer(first,second,1000));
    std::cout << "total=" << first.balance+second.balance << " self-transfer=no-op\n";
}
