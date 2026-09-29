#include "course_test.hpp"
#include <memory>
#include <string>
#include <vector>
#include <utility>

struct Node {
    std::string name;
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;
    explicit Node(std::string value) : name(std::move(value)) {}
};

int main() {
    auto owner = std::make_unique<int>(42);
    auto next = std::move(owner);
    CHECK(owner == nullptr && *next == 42);
    auto root = std::make_shared<Node>("root");
    auto child = std::make_shared<Node>("child");
    child->parent = root;
    root->children.push_back(child);
    const std::weak_ptr<Node> root_observer = root;
    const std::weak_ptr<Node> child_observer = child;
    {
        auto parent = child->parent.lock();
        CHECK(parent && parent->name == "root");
    }
    root.reset();
    CHECK(root_observer.expired());
    CHECK(child->parent.expired());
    CHECK(!child_observer.expired());
    child.reset();
    CHECK(child_observer.expired());
    course::report();
}
