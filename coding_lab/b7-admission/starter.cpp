#include <iostream>
#include <cstddef>

class Admission {
    std::size_t limit_,count_=0;
public:
    explicit Admission(std::size_t limit):limit_(limit){}
    bool submit(int value) {
        // TODO: validate before changing state.
        return false;
    }
    std::size_t size() const { return count_; }
};
