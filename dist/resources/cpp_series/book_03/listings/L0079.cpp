#include <algorithm>
#include <iostream>
#include <vector>
int main(){std::vector<int>w{2,3,4},val{4,5,10};int cap=6;std::vector<int>dp(cap+1);for(std::size_t i=0;i<w.size();++i)for(int c=cap;c>=w[i];--c)dp[c]=std::max(dp[c],dp[c-w[i]]+val[i]);std::cout<<dp[cap]<<'\n';}
