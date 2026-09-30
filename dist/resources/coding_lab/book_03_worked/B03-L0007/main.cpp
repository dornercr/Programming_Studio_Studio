#include <iostream>
long linear(int n){long steps=0;for(int i=0;i<n;++i)++steps;return steps;}
long quadratic(int n){long steps=0;for(int i=0;i<n;++i)for(int j=0;j<n;++j)++steps;return steps;}
int main(){for(int n:{10,20,40})std::cout<<n<<' '<<linear(n)<<' '<<quadratic(n)<<'\n';}
