#include <iostream>
#include <memory>
struct Node{int v;std::unique_ptr<Node>l,r;};
void insert(std::unique_ptr<Node>&n,int x){if(!n){n=std::make_unique<Node>(Node{x});return;}if(x<n->v)insert(n->l,x);else if(x>n->v)insert(n->r,x);}
bool contains(const Node*n,int x){while(n){if(x==n->v)return true;n=x<n->v?n->l.get():n->r.get();}return false;}
int main(){std::unique_ptr<Node>r;for(int x:{5,2,8,3})insert(r,x);std::cout<<std::boolalpha<<contains(r.get(),3)<<' '<<contains(r.get(),7)<<'\n';}
