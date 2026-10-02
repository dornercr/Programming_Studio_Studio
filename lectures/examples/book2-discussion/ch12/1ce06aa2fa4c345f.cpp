#include <array>
#include <cstddef>
#include <iostream>
template<class T,std::size_t N> struct Buffer{ std::array<T,N> data{}; constexpr std::size_t size() const{return N;} };
int main(){ Buffer<int,4> b{{1,2,3,4}}; std::cout<<b.size()<<' '<<b.data[2]<<'\n'; }
