#include <iostream>
#include <memory>
struct Node{int v;std::unique_ptr<Node>l,r;};
void rotate_right(std::unique_ptr<Node>&root){auto old=std::move(root);auto pivot=std::move(old->l);old->l=std::move(pivot->r);pivot->r=std::move(old);root=std::move(pivot);} 
int main(){auto r=std::make_unique<Node>(Node{3});r->l=std::make_unique<Node>(Node{2});r->l->l=std::make_unique<Node>(Node{1});rotate_right(r);std::cout<<r->v<<' '<<r->l->v<<' '<<r->r->v<<'\n';}
