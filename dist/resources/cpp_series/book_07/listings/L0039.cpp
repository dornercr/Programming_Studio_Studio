#include <atomic>
#include <memory>
#include <iostream>

int main() {
    struct Config{int timeout_ms;unsigned generation;};
    std::atomic<std::shared_ptr<const Config>> current{std::make_shared<const Config>(Config{500,1})};
    auto in_flight=current.load();
    auto reload=[&](int timeout){if(timeout<1||timeout>5000)return false;auto old=current.load();
        current.store(std::make_shared<const Config>(Config{timeout,old->generation+1}));return true;};
    bool bad=reload(-1);bool good=reload(250);auto fresh=current.load();
    std::cout<<std::boolalpha<<"invalid="<<bad<<" valid="<<good<<" old="<<in_flight->timeout_ms<<" new="<<fresh->timeout_ms<<'\n';
}
