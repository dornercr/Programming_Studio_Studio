#include <concepts>
#include <iostream>
template<std::integral T> T gcd_like(T a,T b){ while(b!=0){T r=a%b;a=b;b=r;} return a; }
int main(){ std::cout<<gcd_like(48,18)<<'\n'; }
