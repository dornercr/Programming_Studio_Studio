#include <iostream>
#include <stdexcept>
class Range{
    int low_, high_;
public:
    Range(int low,int high):low_(low),high_(high){ if(low>high) throw std::invalid_argument("low > high"); }
    int width() const { return high_-low_; }
};
int main(){ Range r{3,8}; std::cout << r.width() << '\n'; }
