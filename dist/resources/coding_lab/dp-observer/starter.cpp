#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <utility>

class Signal {
    std::vector<std::function<void(int)>> listeners_;
public:
    void subscribe(std::function<void(int)> fn){listeners_.push_back(std::move(fn));}
    void publish(int value){
        // TODO: notify the listeners present when this call begins.
    }
};
