#include "course_test.hpp"
#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

struct Node {
    int value;
    std::vector<std::unique_ptr<Node>> children;
    explicit Node(int v) : value(v) {}
    Node& add(int v) { children.push_back(std::make_unique<Node>(v)); return *children.back(); }
};
int height(const Node* node) {
    if (!node) return -1; // Height measured in edges; a leaf has height zero.
    int longest{-1};
    for (const auto& child : node->children) longest = std::max(longest, height(child.get()));
    return longest + 1;
}
void preorder(const Node& node, std::vector<int>& result) {
    result.push_back(node.value);
    for (const auto& child : node.children) preorder(*child, result);
}
void postorder(const Node& node, std::vector<int>& result) {
    for (const auto& child : node.children) postorder(*child, result);
    result.push_back(node.value);
}
std::vector<int> breadth_first(const Node& root) {
    std::vector<int> result;
    std::queue<const Node*> pending;
    pending.push(&root);
    while (!pending.empty()) {
        const Node* node = pending.front(); pending.pop();
        result.push_back(node->value);
        for (const auto& child : node->children) pending.push(child.get());
    }
    return result;
}

int main() {
    Node root{1};
    Node& left = root.add(2);
    root.add(3);
    left.add(4); left.add(5);
    std::vector<int> pre, post;
    preorder(root, pre); postorder(root, post);
    CHECK(pre == std::vector<int>({1, 2, 4, 5, 3}));
    CHECK(post == std::vector<int>({4, 5, 2, 3, 1}));
    CHECK(breadth_first(root) == std::vector<int>({1, 2, 3, 4, 5}));
    CHECK(height(&root) == 2 && height(nullptr) == -1);
    course::report();
}
