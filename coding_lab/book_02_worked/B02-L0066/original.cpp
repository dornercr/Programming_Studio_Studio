#include <deque>
#include <iostream>
#include <vector>
int main(){ std::vector<int> history{1,2}; history.push_back(3); std::deque<int> work{2,3}; work.push_front(1); std::cout<<history.front()<<' '<<history.back()<<' '<<work.front()<<'\n'; }
