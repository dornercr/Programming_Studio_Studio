#include <iostream>
#include <vector>
struct Buffer{ std::vector<int> data; };
int main(){ Buffer a{{1,2,3}}; Buffer b=a; b.data[0]=9; std::cout<<a.data[0]<<' '<<b.data[0]<<'\n'; }
