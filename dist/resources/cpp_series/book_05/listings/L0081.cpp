#include <cstddef>
#include <iostream>
#include <vector>

int main(){
 std::vector<int> v(1'000'000,1); long long a=0,b=0,c=0,d=0;
 for(std::size_t i=0;i+3<v.size();i+=4){a+=v[i];b+=v[i+1];c+=v[i+2];d+=v[i+3];}
 std::cout<<a+b+c+d<<"\n";
}
