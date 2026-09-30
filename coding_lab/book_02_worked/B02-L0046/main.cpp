#include <iostream>
#include <string>
template<class T> T maximum(T a,T b){ return b<a?a:b; }
int main(){ std::cout<<maximum(3,7)<<' '<<maximum(std::string{"Ada"},std::string{"Grace"})<<'\n'; }
