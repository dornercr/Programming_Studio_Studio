#include <iostream>
#include <memory>
#include <utility>

struct Node{int value;std::unique_ptr<Node> next;explicit Node(int v):value(v){}};
std::unique_ptr<Node> reverseChain(std::unique_ptr<Node> head){
    std::unique_ptr<Node> reversed;
    while(head){
        auto rest=std::move(head->next); // Save ownership of the unvisited suffix.
        head->next=std::move(reversed);
        reversed=std::move(head);
        head=std::move(rest);
    }
    return reversed;
}
