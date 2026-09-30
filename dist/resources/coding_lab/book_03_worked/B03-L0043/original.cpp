#include "course_test.hpp"
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <vector>
#include <utility>

class SearchTree {
    struct Node {
        int key;
        std::unique_ptr<Node> left, right;
        explicit Node(int k) : key(k) {}
    };
    std::unique_ptr<Node> root_;
    static bool insert(std::unique_ptr<Node>& node, int key) {
        if (!node) { node = std::make_unique<Node>(key); return true; }
        if (key == node->key) return false;
        return insert(key < node->key ? node->left : node->right, key);
    }
    static bool erase(std::unique_ptr<Node>& node, int key) {
        if (!node) return false;
        if (key < node->key) return erase(node->left, key);
        if (key > node->key) return erase(node->right, key);
        if (!node->left) { auto old = std::move(node); node = std::move(old->right); }
        else if (!node->right) { auto old = std::move(node); node = std::move(old->left); }
        else {
            Node* successor = node->right.get();
            while (successor->left) successor = successor->left.get();
            node->key = successor->key;
            return erase(node->right, successor->key);
        }
        return true;
    }
    static void inorder(const Node* node, std::vector<int>& result) {
        if (!node) return;
        inorder(node->left.get(), result); result.push_back(node->key);
        inorder(node->right.get(), result);
    }
public:
    bool insert(int key) { return insert(root_, key); }
    bool erase(int key) { return erase(root_, key); }
    bool contains(int key) const {
        const Node* node = root_.get();
        while (node) {
            if (key == node->key) return true;
            node = key < node->key ? node->left.get() : node->right.get();
        }
        return false;
    }
    std::vector<int> sorted() const {
        std::vector<int> result; inorder(root_.get(), result); return result;
    }
};

int main() {
    SearchTree actual;
    std::set<int> expected;
    std::mt19937 random{7};
    for (int step = 0; step < 3000; ++step) {
        const int key = static_cast<int>(random() % 100) - 50;
        if (random() % 2 == 0) CHECK(actual.insert(key) == expected.insert(key).second);
        else CHECK(actual.erase(key) == (expected.erase(key) != 0));
        CHECK(actual.sorted() == std::vector<int>(expected.begin(), expected.end()));
        CHECK(actual.contains(key) == expected.contains(key));
    }
    SearchTree three;
    for (int k : {2, 1, 3}) three.insert(k);
    CHECK(three.erase(2)); CHECK(three.sorted() == std::vector<int>({1, 3}));
    course::report();
}
