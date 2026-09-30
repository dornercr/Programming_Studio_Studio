#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

class Stock{int count_;public:
 explicit Stock(int n):count_(n){if(n<0||n>100)throw std::invalid_argument("range");}
 int count()const{return count_;}
 bool take(int amount){
 // BUG: rejects exact depletion.
 if(amount<0||amount>=count_)return false;count_-=amount;return true;
 }
};

int main() {
    Stock s(5);
    bool ok=s.take(5);
    std::cout<<std::boolalpha<<ok<<" "<<s.count()<<"\n";
    }
