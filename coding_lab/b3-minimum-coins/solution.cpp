#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

int minimumCoins(const std::vector<int>& coins,int target){
    if(target<0||target>1000) throw std::invalid_argument("target");
    for(int c:coins) if(c<=0) throw std::invalid_argument("coin");
    const int missing=target+1;std::vector<int> dp(target+1,missing);dp[0]=0;
    for(int amount=1;amount<=target;++amount)
        for(int c:coins) if(c<=amount&&dp[amount-c]!=missing)
            dp[amount]=std::min(dp[amount],dp[amount-c]+1);
    return dp[target]==missing?-1:dp[target];
}
