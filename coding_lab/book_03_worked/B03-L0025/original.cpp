#include <iostream>
#include <queue>
int main(){std::queue<int>q;for(int x:{10,20,30})q.push(x);while(!q.empty()){std::cout<<q.front()<<' ';q.pop();}}
