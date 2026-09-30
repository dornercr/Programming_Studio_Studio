#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cstddef>

class Snapshot{std::vector<int> values_;public:explicit Snapshot(std::vector<int> v):values_(std::move(v)){}int get(std::size_t i)const{return values_.at(i);}void set(std::size_t i,int v){values_.at(i)=v;}std::size_t size()const{return values_.size();}};
