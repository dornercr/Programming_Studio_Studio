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

#include <memory>
#include <optional>
#include <vector>
#include <utility>

class SinglyList {
    struct Node { int value; std::unique_ptr<Node> next; };
    std::unique_ptr<Node> head_;
public:
    SinglyList() = default;
    SinglyList(const SinglyList&) = delete;
    SinglyList& operator=(const SinglyList&) = delete;
    ~SinglyList() { while (head_) { auto next = std::move(head_->next); head_ = std::move(next); } }
    void push_front(int value) {
        auto node = std::make_unique<Node>();
        node->value = value;
        node->next = std::move(head_);
        head_ = std::move(node);
    }
    bool erase_first(int value) {
        auto* owner = &head_;
        while (*owner && (*owner)->value != value) owner = &(*owner)->next;
        if (!*owner) return false;
        auto victim = std::move(*owner);
        *owner = std::move(victim->next);
        return true;
    }
    std::vector<int> values() const {
        std::vector<int> result;
        for (auto* node = head_.get(); node; node = node->next.get()) result.push_back(node->value);
        return result;
    }
};
class DoublyList {
    struct Node { int value; Node* previous{}; std::unique_ptr<Node> next; };
    std::unique_ptr<Node> head_;
    Node* tail_{}; // Observer, not an additional owner.
public:
    DoublyList() = default;
    DoublyList(const DoublyList&) = delete;
    DoublyList& operator=(const DoublyList&) = delete;
    ~DoublyList() { while (head_) { auto next = std::move(head_->next); head_ = std::move(next); } }
    void push_back(int value) {
        auto node = std::make_unique<Node>();
        node->value = value; node->previous = tail_;
        Node* new_tail = node.get();
        if (tail_) tail_->next = std::move(node);
        else head_ = std::move(node);
        tail_ = new_tail;
    }
    bool erase_first(int value) {
        auto* owner = &head_;
        while (*owner && (*owner)->value != value) owner = &(*owner)->next;
        if (!*owner) return false;
        auto victim = std::move(*owner);
        Node* previous = victim->previous;
        *owner = std::move(victim->next);
        if (*owner) (*owner)->previous = previous;
        else tail_ = previous;
        return true;
    }
    std::vector<int> forward() const {
        std::vector<int> result;
        for (auto* p = head_.get(); p; p = p->next.get()) result.push_back(p->value);
        return result;
    }
    std::vector<int> backward() const {
        std::vector<int> result;
        for (auto* p = tail_; p; p = p->previous) result.push_back(p->value);
        return result;
    }
};

int main() {
    SinglyList single;
    for (int value : {3, 2, 1}) single.push_front(value);
    CHECK(single.values() == std::vector<int>({1, 2, 3}));
    CHECK(single.erase_first(2));
    CHECK(!single.erase_first(9));
    CHECK(single.erase_first(1) && single.erase_first(3));
    CHECK(single.values().empty());
    DoublyList dual;
    for (int value : {1, 2, 3}) dual.push_back(value);
    CHECK(dual.backward() == std::vector<int>({3, 2, 1}));
    CHECK(dual.erase_first(2));
    CHECK(dual.forward() == std::vector<int>({1, 3}));
    CHECK(dual.backward() == std::vector<int>({3, 1}));
    CHECK(dual.erase_first(3) && dual.erase_first(1));
    CHECK(dual.forward().empty() && dual.backward().empty());
    dual.push_back(4);
    CHECK(dual.backward() == std::vector<int>({4}));
    course::report();
}
