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

class Interval {
    int low_, high_;
public:
    Interval(int low, int high) : low_(low), high_(high) {
        if (low < -100 || high > 100 || low > high) throw std::invalid_argument("range");
    }
    int low() const { return low_; }
    int high() const { return high_; }
};

int main() {
    try{Interval r(3,3);
    std::cout<<r.low()<<" "<<r.high()<<"\n";
    }catch(const std::invalid_argument&){std::cout<<"range\n";
    }}
