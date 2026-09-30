#include <algorithm>
#include <iostream>
#include <vector>
int main(){ std::vector<int> v{9,2,7,1}; std::sort(v.begin(),v.end()); std::cout<<std::boolalpha<<std::binary_search(v.begin(),v.end(),7)<<'\n'; }
