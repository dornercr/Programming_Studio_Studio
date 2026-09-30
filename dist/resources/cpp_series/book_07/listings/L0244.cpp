#include <array>
#include <iostream>

int main() {
    auto transfer=[](int&from,int&to,int amount){if(amount<0||amount>from)return false;from-=amount;to+=amount;return true;};
    unsigned checked=0;
    for(int amount:std::array{-1,0,3,5,6}){int from=5,to=2;bool ok=transfer(from,to,amount);
        bool expected=amount>=0&&amount<=5;
        if(ok!=expected||from+to!=7||from!=(expected?5-amount:5)||to!=(expected?2+amount:2))return 1;++checked;}
    std::cout<<"five cases preserve balances and rejection semantics\n";
}
