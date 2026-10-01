#include <iostream>
#include <vector>

struct Node {
    int own_minutes;
    std::vector<Node> children;
};
int recursive_total(const Node& node) {
    int total = node.own_minutes;
    for (const Node& child : node.children) {
        total += recursive_total(child); // Apply the same question below.
    }
    return total;
}
int main() {
    const Node project{0, {{0, {{10, {}}, {20, {}}}}, {5, {}}}};
    int shallow = project.own_minutes;
    for (const Node& child : project.children) shallow += child.own_minutes;
    std::cout << "shallow=" << shallow << '\n';
    std::cout << "recursive=" << recursive_total(project) << '\n';
}
