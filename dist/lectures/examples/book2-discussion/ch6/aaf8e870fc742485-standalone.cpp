// Exact original book listing B02-L0110
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

// Original book listing B02-L0028
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
