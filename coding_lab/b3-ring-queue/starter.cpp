#include <iostream>
#include <vector>
#include <optional>
#include <cstddef>

class RingQueue{std::vector<int> data_;std::size_t head_=0,count_=0;public:explicit RingQueue(std::size_t n):data_(n){}bool push(int v){if(count_==data_.size())return false;data_[(head_+count_)%data_.size()]=v;++count_;return true;}std::optional<int> pop(){if(!count_)return std::nullopt;int v=data_[head_];--count_;return v;}std::size_t size()const{return count_;}};
