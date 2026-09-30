#include <iostream>
#include <concepts>
#include <numeric>
#include <limits>

template<std::integral T>requires(!std::same_as<T,bool>)T midpoint(T a,T b){return std::midpoint(a,b);}
