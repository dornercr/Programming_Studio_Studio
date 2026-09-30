#include <iostream>
class Counter{
    int value_{};
public:
    void increment(){ ++value_; }
    int value() const { return value_; }
};
int main(){ Counter c; c.increment(); std::cout << c.value() << '\n'; }
