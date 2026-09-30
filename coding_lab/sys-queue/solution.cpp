#include <iostream>
#include <deque>
#include <optional>
#include <cstddef>

class BoundedQueue {
    std::deque<int> values_;
    std::size_t capacity_;
public:
    explicit BoundedQueue(std::size_t capacity):capacity_(capacity){}
    bool push(int value){
        if(values_.size()>=capacity_) return false;
        values_.push_back(value); return true;
    }
    std::optional<int> pop(){
        if(values_.empty()) return std::nullopt;
        int value=values_.front(); values_.pop_front(); return value;
    }
    std::size_t size() const { return values_.size(); }
};
