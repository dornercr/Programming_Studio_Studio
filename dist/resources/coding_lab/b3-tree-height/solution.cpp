#include <iostream>
#include <memory>
#include <algorithm>
#include <cstddef>

struct Node{std::unique_ptr<Node> left,right;};
std::size_t height(const Node* p){
    if(!p) return 0;
    return 1+std::max(height(p->left.get()),height(p->right.get()));
}
