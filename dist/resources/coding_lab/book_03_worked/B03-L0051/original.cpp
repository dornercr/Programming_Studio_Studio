#include <iostream>
#include <queue>
#include <string>
#include <vector>
struct Job{int priority;std::string name;};struct Compare{bool operator()(const Job&a,const Job&b)const{return a.priority<b.priority;}};
int main(){std::priority_queue<Job,std::vector<Job>,Compare>q;q.push({2,"normal"});q.push({5,"urgent"});q.push({1,"low"});while(!q.empty()){std::cout<<q.top().name<<' ';q.pop();}}
