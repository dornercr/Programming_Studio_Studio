#include <iostream>
#include <vector>

int main(){
 constexpr std::size_t N=1<<22; std::vector<int> v(N,1); long long sum=0;
 for(std::size_t i=0;i<N;i+=16) sum+=v[i];
 std::cout<<sum<<"\n";
}
