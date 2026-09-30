#include <iostream>
#include <string>
#include <unordered_set>
#include <stdexcept>

class IdempotentCounter {
    std::unordered_set<std::string> seen_;
    int total_=0;
public:
    bool apply(const std::string& key,int delta) {
        // TODO: reject empty keys, then apply each key once.
        total_+=delta; return true;
    }
    int total() const { return total_; }
};
