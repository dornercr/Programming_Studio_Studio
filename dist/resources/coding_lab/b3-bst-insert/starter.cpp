#include <iostream>
#include <memory>

struct Node{int key;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
bool insert(std::unique_ptr<Node>& root,int k){if(!root){root=std::make_unique<Node>(k);return true;}return false;}
