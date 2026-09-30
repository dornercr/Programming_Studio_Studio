#include <iostream>
#include <memory>
#include <algorithm>
#include <cstddef>

struct Node{std::unique_ptr<Node> left,right;};std::size_t height(const Node* p){return p?1:0;}
