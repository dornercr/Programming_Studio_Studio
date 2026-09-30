#include <iostream>
struct Node{int v;Node*l{};Node*r{};};
void pre(Node*n){if(!n)return;std::cout<<n->v;pre(n->l);pre(n->r);}void in(Node*n){if(!n)return;in(n->l);std::cout<<n->v;in(n->r);}void post(Node*n){if(!n)return;post(n->l);post(n->r);std::cout<<n->v;}
int main(){Node a{2},b{1},c{3};a.l=&b;a.r=&c;pre(&a);std::cout<<' ';in(&a);std::cout<<' ';post(&a);std::cout<<'\n';}
