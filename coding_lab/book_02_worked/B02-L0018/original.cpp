#include "course_test.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

class Buffer {
    std::size_t size_{};
    std::unique_ptr<int[]> data_;
public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(size ? std::make_unique<int[]>(size) : nullptr) {}
    Buffer(const Buffer& other) : Buffer(other.size_) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
    }
    Buffer& operator=(const Buffer& other) {
        Buffer copy{other};
        swap(copy);
        return *this;
    }
    void swap(Buffer& other) noexcept {
        std::swap(size_, other.size_);
        data_.swap(other.data_);
    }
    std::size_t size() const noexcept { return size_; }
    int& at(std::size_t index) {
        if (index >= size_) throw std::out_of_range("buffer index");
        return data_[index];
    }
    const int& at(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("buffer index");
        return data_[index];
    }
};

int main() {
    Buffer original{3};
    original.at(0) = 7;
    Buffer copy{original};
    copy.at(0) = 99;
    CHECK(original.at(0) == 7 && copy.at(0) == 99);
    Buffer assigned{1};
    assigned = original;
    CHECK(assigned.size() == 3 && assigned.at(0) == 7);
    const Buffer& same = assigned;
    assigned = same;
    CHECK(assigned.at(0) == 7);
    Buffer empty{0};
    assigned = empty;
    CHECK(assigned.size() == 0);
    CHECK(course::throws<std::out_of_range>([&] { (void)assigned.at(0); }));
    course::report();
}
