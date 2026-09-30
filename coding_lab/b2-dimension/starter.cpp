#include <iostream>
#include <stdexcept>

class Dimension{int value_;public:explicit Dimension(int n):value_(n){}void set(int n){value_=n;}int value()const{return value_;}};
