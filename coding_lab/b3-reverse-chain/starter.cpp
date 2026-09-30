#include <iostream>
#include <memory>
#include <utility>

struct Node{int value;std::unique_ptr<Node> next;explicit Node(int v):value(v){}};
std::unique_ptr<Node> reverseChain(std::unique_ptr<Node> head){return head;}
