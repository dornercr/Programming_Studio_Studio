#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
class Dyn{std::unique_ptr<int[]>d_;std::size_t size_{},cap_{};public:void push(int x){if(size_==cap_){std::size_t nc=cap_?cap_*2:1;auto n=std::make_unique<int[]>(nc);std::copy_n(d_.get(),size_,n.get());d_=std::move(n);cap_=nc;}d_[size_++]=x;}std::size_t size()const{return size_;}std::size_t capacity()const{return cap_;}};
int main(){Dyn d;for(int i=0;i<5;++i){d.push(i);std::cout<<d.size()<<'/'<<d.capacity()<<' ';}}
