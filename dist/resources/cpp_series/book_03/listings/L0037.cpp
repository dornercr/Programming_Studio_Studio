#include <algorithm>
#include <iostream>
#include <memory>
struct Node{int value;std::unique_ptr<Node>left,right;};
int height(const Node*n){return n?1+std::max(height(n->left.get()),height(n->right.get())):0;}
bool leaf(const Node*n){return n&&!n->left&&!n->right;}
int main(){Node root{1,std::make_unique<Node>(Node{2}),std::make_unique<Node>(Node{3})};std::cout<<height(&root)<<' '<<std::boolalpha<<leaf(root.left.get())<<'\n';}
