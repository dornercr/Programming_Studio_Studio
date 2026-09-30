#include <iostream>
#include <memory>
#include <vector>

struct Node{std::weak_ptr<Node> parent;std::shared_ptr<Node> parentOwner;std::vector<std::shared_ptr<Node>> children;};std::shared_ptr<Node> makeChild(const std::shared_ptr<Node>& parent){auto child=std::make_shared<Node>();child->parent=parent;child->parentOwner=parent;parent->children.push_back(child);return child;}
