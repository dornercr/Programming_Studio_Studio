#include <iostream>
#include <deque>
#include <optional>
#include <cstddef>

class BoundedQueue {
    std::deque<int> values_;
    std::size_t capacity_;
public:
    explicit BoundedQueue(std::size_t capacity):capacity_(capacity){}
    bool push(int value){ /* TODO */ return false; }
    std::optional<int> pop(){ /* TODO */ return std::nullopt; }
    std::size_t size() const { return values_.size(); }
};
