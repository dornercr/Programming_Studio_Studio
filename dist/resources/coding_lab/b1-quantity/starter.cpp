#include <iostream>

class Quantity{int value_{};public:bool add(int amount){if(amount<=0||amount>1000-value_)return false;value_+=amount;return true;}bool take(int amount){value_-=amount;return true;}int value()const{return value_;}};
