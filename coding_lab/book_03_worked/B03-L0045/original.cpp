#include <iostream>
#include <memory>
struct Node{int v;std::unique_ptr<Node>r;};
int main(){auto root=std::make_unique<Node>(Node{1});Node*p=root.get();for(int x=2;x<=6;++x){p->r=std::make_unique<Node>(Node{x});p=p->r.get();}int depth=0;for(Node*n=root.get();n;n=n->r.get())++depth;std::cout<<depth<<'\n';}
