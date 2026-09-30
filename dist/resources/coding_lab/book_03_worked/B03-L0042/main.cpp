#include <memory>
#include <iostream>
struct Node{int v;std::unique_ptr<Node>l,r;};
void insert(std::unique_ptr<Node>&n,int x){if(!n)n=std::make_unique<Node>(Node{x});else if(x<n->v)insert(n->l,x);else if(x>n->v)insert(n->r,x);} 
void erase(std::unique_ptr<Node>&n,int x){if(!n)return;if(x<n->v)return erase(n->l,x);if(x>n->v)return erase(n->r,x);if(!n->l){n=std::move(n->r);return;}if(!n->r){n=std::move(n->l);return;}Node*s=n->r.get();while(s->l)s=s->l.get();n->v=s->v;erase(n->r,s->v);} 
void in(const Node*n){if(!n)return;in(n->l.get());std::cout<<n->v<<' ';in(n->r.get());}
int main(){std::unique_ptr<Node>r;for(int x:{5,2,8,1,3,7,9})insert(r,x);erase(r,5);in(r.get());}
