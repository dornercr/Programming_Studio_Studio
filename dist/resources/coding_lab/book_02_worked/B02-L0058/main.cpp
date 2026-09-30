#include <algorithm>
#include <iostream>
#include <vector>
int main(){ std::vector<int> v{2,7,4,9}; int threshold=5; auto count=std::count_if(v.begin(),v.end(),[threshold](int x){return x>threshold;}); std::cout<<count<<'\n'; }
