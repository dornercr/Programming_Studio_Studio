#include <cassert>
#include <iostream>
#include <mutex>
#include <future>



int main() {
    int first = 100, second = 100;
    std::mutex mutex;
    const auto move = [&](bool forward) {
        for(int i=0;i<100;++i) {
            std::lock_guard<std::mutex> lock(mutex);
            int& from = forward ? first : second;
            int& to = forward ? second : first;
            assert(from > 0);
            --from; ++to;
            assert(first+second == 200);
        }
    };
    auto a = std::async(std::launch::async,move,true);
    auto b = std::async(std::launch::async,move,false);
    a.get(); b.get();
    assert(first == 100 && second == 100);
    std::cout << "balances=" << first << ',' << second << " total=" << first+second << '\n';
}
