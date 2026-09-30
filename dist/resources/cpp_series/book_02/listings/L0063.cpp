#include <iostream>
#include <vector>
int main(){ std::vector<int> v; v.reserve(2); v.push_back(1); auto before=v.data(); v.push_back(2); auto stable=v.data()==before; v.push_back(3); auto reallocated=v.data()!=before; std::cout<<std::boolalpha<<stable<<' '<<reallocated<<'\n'; }
