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

class Stock {
    int count_;
public:
    explicit Stock(int count) : count_(count) {
        if (count < 0 || count > 100) throw std::invalid_argument("range");
    }
    int count() const { return count_; }
    bool take(int amount) {
        if (amount < 0 || amount > count_) return false;
        count_ -= amount; // Private state changes only after validation.
        return true;
    }
};

int main() {
    Stock s(5);
    bool ok=s.take(5);
    std::cout<<std::boolalpha<<ok<<" "<<s.count()<<"\n";
    }
