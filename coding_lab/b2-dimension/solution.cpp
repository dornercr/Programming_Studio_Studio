#include <iostream>
#include <stdexcept>

class Dimension{int value_;public:explicit Dimension(int n):value_(n){if(n<=0)throw std::invalid_argument("dimension");}void set(int n){if(n<=0)throw std::invalid_argument("dimension");value_=n;}int value()const{return value_;}};
