// Shared test support from Book II, B02-L0110.
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

#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>
#include <utility>

class AvlTree {
    struct Node {
        int key, height{1};
        std::unique_ptr<Node> left, right;
        explicit Node(int k) : key(k) {}
    };
    std::unique_ptr<Node> root_;
    static int height(const std::unique_ptr<Node>& p) { return p ? p->height : 0; }
    static void update(Node& p) { p.height = 1 + std::max(height(p.left), height(p.right)); }
    static int balance(const Node& p) { return height(p.left) - height(p.right); }
    static std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> top) {
        auto promoted = std::move(top->left);
        top->left = std::move(promoted->right);
        update(*top);
        promoted->right = std::move(top);
        update(*promoted);
        return promoted;
    }
    static std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> top) {
        auto promoted = std::move(top->right);
        top->right = std::move(promoted->left);
        update(*top);
        promoted->left = std::move(top);
        update(*promoted);
        return promoted;
    }
    static std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int key,
                                        std::unique_ptr<Node>& fresh) noexcept {
        if (!node) return std::move(fresh);
        if (key < node->key) node->left = insert(std::move(node->left), key, fresh);
        else if (key > node->key) node->right = insert(std::move(node->right), key, fresh);
        else return node;
        update(*node);
        if (balance(*node) > 1) {
            if (key > node->left->key) node->left = rotate_left(std::move(node->left));
            return rotate_right(std::move(node));
        }
        if (balance(*node) < -1) {
            if (key < node->right->key) node->right = rotate_right(std::move(node->right));
            return rotate_left(std::move(node));
        }
        return node;
    }
    static int validate(const Node* node, std::optional<int> low, std::optional<int> high) {
        if (!node) return 0;
        CHECK(!low || *low < node->key);
        CHECK(!high || node->key < *high);
        const int left = validate(node->left.get(), low, node->key);
        const int right = validate(node->right.get(), node->key, high);
        CHECK(std::abs(left - right) <= 1);
        CHECK(node->height == 1 + std::max(left, right));
        return node->height;
    }
public:
    void insert(int key) {
        auto fresh = std::make_unique<Node>(key);
        root_ = insert(std::move(root_), key, fresh);
    }
    int validate() const { return validate(root_.get(), {}, {}); }
};
struct RedBlackNode {
    bool red;
    const RedBlackNode* left{};
    const RedBlackNode* right{};
};
int black_height(const RedBlackNode* node) {
    if (!node) return 1; // Null leaves are black in this representation.
    if (node->red && ((node->left && node->left->red) ||
                      (node->right && node->right->red))) return -1;
    const int left = black_height(node->left), right = black_height(node->right);
    if (left < 0 || left != right) return -1;
    return left + (node->red ? 0 : 1);
}
bool valid_colors(const RedBlackNode* root) {
    return (!root || !root->red) && black_height(root) >= 0;
}

int main() {
    for (const auto& order : {std::vector<int>{3, 2, 1}, {1, 2, 3}, {3, 1, 2}, {1, 3, 2}}) {
        AvlTree tree;
        for (int key : order) { tree.insert(key); tree.validate(); }
        CHECK(tree.validate() == 2);
    }
    AvlTree ordered;
    for (int key = 0; key < 1000; ++key) { ordered.insert(key); ordered.validate(); }
    CHECK(ordered.validate() < 20);
    const RedBlackNode left{true}, right{true};
    const RedBlackNode valid{false, &left, &right};
    const RedBlackNode invalid_root{true, &left, &right};
    CHECK(valid_colors(&valid)); CHECK(!valid_colors(&invalid_root));
    course::report();
}
