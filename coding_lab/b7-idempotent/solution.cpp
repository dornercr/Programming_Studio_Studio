#include <iostream>
#include <string>
#include <unordered_set>
#include <stdexcept>

class IdempotentCounter {
    std::unordered_set<std::string> seen_;
    int total_=0;
public:
    bool apply(const std::string& key,int delta) {
        if(key.empty()) throw std::invalid_argument("empty key");
        if(!seen_.insert(key).second) return false;
        total_+=delta; return true;
    }
    int total() const { return total_; }
};
