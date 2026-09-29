#include <iostream>
#include <type_traits>
template<class T> void describe(const T&){ if constexpr(std::is_integral_v<T>) std::cout<<"integral\n"; else std::cout<<"other\n"; }
int main(){ describe(3); describe(3.5); }
