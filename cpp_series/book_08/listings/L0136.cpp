#include <deque>
#include <algorithm>
#include <iostream>
#include <utility>

int main() {
    std::deque<std::pair<int,int>> recommendations;
    auto stabilized=[&](int time,int desired){recommendations.emplace_back(time,desired);
        while(!recommendations.empty()&&recommendations.front().first<time-300)recommendations.pop_front();
        int result=desired;for(auto[t,n]:recommendations)result=std::max(result,n);return result;};
    std::cout<<stabilized(0,6)<<' '<<stabilized(10,2)<<' '<<stabilized(301,2)<<'\n';
}
