#include <iostream>
struct Node{int value;Node*prev{};Node*next{};};
int main(){Node a{1},b{2},c{3};a.next=&b;b.prev=&a;b.next=&c;c.prev=&b;for(Node*p=&c;p;p=p->prev)std::cout<<p->value<<' ';}
