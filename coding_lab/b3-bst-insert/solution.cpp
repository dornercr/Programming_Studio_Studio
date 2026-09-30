#include <iostream>
#include <memory>

struct Node{int key;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
bool insert(std::unique_ptr<Node>& root,int k){
    if(!root){root=std::make_unique<Node>(k);return true;}
    if(k<root->key) return insert(root->left,k);
    if(k>root->key) return insert(root->right,k);
    return false;
}
