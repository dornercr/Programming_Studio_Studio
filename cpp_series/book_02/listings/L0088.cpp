#include <iostream>
#include <memory>
#include <vector>
struct Node{ int value; Node* parent{}; std::vector<std::unique_ptr<Node>> children; explicit Node(int v):value(v){} Node& add(int v){auto n=std::make_unique<Node>(v);n->parent=this;children.push_back(std::move(n));return *children.back();} };
int main(){ auto root=std::make_unique<Node>(1); Node& child=root->add(2); std::cout<<child.parent->value<<'\n'; }
