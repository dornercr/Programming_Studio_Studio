#include <iostream>
#include <utility>
#include <vector>
int main(){ std::vector<int> a(100000,7); auto before=a.data(); std::vector<int> b=std::move(a); std::cout<<std::boolalpha<<(b.data()==before)<<' '<<b.size()<<'\n'; }
