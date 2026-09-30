#include <iostream>
#include <memory>
struct Node{int value;std::unique_ptr<Node>next;};
int main(){std::unique_ptr<Node>head;for(int x:{1,2,3}){auto n=std::make_unique<Node>();n->value=x;n->next=std::move(head);head=std::move(n);}for(Node*p=head.get();p;p=p->next.get())std::cout<<p->value<<' ';}
