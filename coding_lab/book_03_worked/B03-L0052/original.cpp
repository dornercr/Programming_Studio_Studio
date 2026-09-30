#include "course_test.hpp"
#include <algorithm>
#include <functional>
#include <optional>
#include <queue>
#include <random>
#include <vector>
#include <utility>

class MinHeap {
    std::vector<int> values_;
    void sift_down(std::size_t parent) {
        while (parent < values_.size() / 2) {
            std::size_t child = 2 * parent + 1;
            if (child + 1 < values_.size() && values_[child + 1] < values_[child]) ++child;
            if (!(values_[child] < values_[parent])) break;
            std::swap(values_[child], values_[parent]);
            parent = child;
        }
    }
public:
    MinHeap() = default;
    explicit MinHeap(std::vector<int> values) : values_(std::move(values)) {
        for (std::size_t i = values_.size() / 2; i > 0; --i) sift_down(i - 1);
    }
    void push(int value) {
        values_.push_back(value);
        std::size_t child = values_.size() - 1;
        while (child > 0) {
            const std::size_t parent = (child - 1) / 2;
            if (!(values_[child] < values_[parent])) break;
            std::swap(values_[child], values_[parent]); child = parent;
        }
    }
    std::optional<int> pop() {
        if (values_.empty()) return std::nullopt;
        const int result = values_.front();
        values_.front() = values_.back(); values_.pop_back();
        if (!values_.empty()) sift_down(0);
        return result;
    }
    bool valid() const {
        for (std::size_t i = 1; i < values_.size(); ++i) {
            if (values_[i] < values_[(i - 1) / 2]) return false;
        }
        return true;
    }
};

int main() {
    MinHeap heap{{9, 3, 7, 1, 1}};
    CHECK(heap.valid());
    for (int value : {1, 1, 3, 7, 9}) CHECK(heap.pop() == value);
    CHECK(!heap.pop());
    std::priority_queue<int, std::vector<int>, std::greater<int>> reference;
    std::mt19937 random{42};
    for (int i = 0; i < 2000; ++i) {
        if (reference.empty() || random() % 2 == 0) {
            const int value = static_cast<int>(random() % 100);
            heap.push(value); reference.push(value);
        } else {
            CHECK(heap.pop() == reference.top()); reference.pop();
        }
        CHECK(heap.valid());
    }
    course::report();
}
