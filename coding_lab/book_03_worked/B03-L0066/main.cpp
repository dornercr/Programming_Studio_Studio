#include <iostream>
#include <vector>
void insertion(std::vector<int>&a){for(std::size_t i=1;i<a.size();++i){int key=a[i];std::size_t j=i;while(j>0&&key<a[j-1]){a[j]=a[j-1];--j;}a[j]=key;}}
int main(){std::vector<int>a{5,2,4,1};insertion(a);for(int x:a)std::cout<<x<<' ';}
