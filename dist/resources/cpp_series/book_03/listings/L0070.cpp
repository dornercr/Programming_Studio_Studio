#include <iostream>
struct Node{int v;Node*l{};Node*r{};};
int sum(const Node*n){if(!n)return 0;return n->v+sum(n->l)+sum(n->r);}int main(){Node a{1},b{2},c{3};a.l=&b;a.r=&c;std::cout<<sum(&a)<<'\n';}
