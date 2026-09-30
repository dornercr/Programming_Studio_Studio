#include <iostream>

class Money{int cents_;public:explicit Money(int n):cents_(n){}int cents()const{return cents_;}friend Money operator+(Money a,Money b){return Money(a.cents_+b.cents_);}friend bool operator==(Money a,Money b){return a.cents_==b.cents_;}};
