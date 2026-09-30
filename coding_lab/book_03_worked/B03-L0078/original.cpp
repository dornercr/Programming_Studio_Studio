#include <iostream>
#include <vector>
long fib(int n,std::vector<long>&memo){if(n<2)return n;if(memo[n]!=-1)return memo[n];return memo[n]=fib(n-1,memo)+fib(n-2,memo);}int main(){std::vector<long>m(41,-1);std::cout<<fib(40,m)<<'\n';}
