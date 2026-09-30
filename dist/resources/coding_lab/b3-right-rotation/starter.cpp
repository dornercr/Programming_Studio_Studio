#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <utility>

struct Node{int key,height=1;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
int h(const std::unique_ptr<Node>& p){return p?p->height:0;}
void update(Node& p){p.height=1+std::max(h(p.left),h(p.right));}
std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> root){if(!root||!root->left)throw std::invalid_argument("rotation");return root;}
