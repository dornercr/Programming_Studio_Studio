#include <iostream>
#include <cstddef>

class Admission {
    std::size_t limit_,count_=0;
public:
    explicit Admission(std::size_t limit):limit_(limit){}
    bool submit(int value) {
        if(value<0||value>100||count_>=limit_) return false;
        ++count_; return true;
    }
    std::size_t size() const { return count_; }
};
