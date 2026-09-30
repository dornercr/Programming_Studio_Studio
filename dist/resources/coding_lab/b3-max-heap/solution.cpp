#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

class MaxHeap{std::vector<int> v_;
public:
    void push(int x){v_.push_back(x);std::push_heap(v_.begin(),v_.end());}
    std::optional<int> pop(){
        if(v_.empty()) return std::nullopt;
        std::pop_heap(v_.begin(),v_.end());
        int x=v_.back();v_.pop_back();return x;
    }
};
