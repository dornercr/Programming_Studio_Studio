#include "course_test.hpp"
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class Buffer {
    std::size_t size_{};
    std::unique_ptr<int[]> data_;
public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(size ? std::make_unique<int[]>(size) : nullptr) {}
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept
        : size_(std::exchange(other.size_, 0)), data_(std::move(other.data_)) {}
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }
    std::size_t size() const noexcept { return size_; }
    int& at(std::size_t index) {
        if (index >= size_) throw std::out_of_range("buffer index");
        return data_[index];
    }
};

int main() {
    static_assert(std::is_nothrow_move_constructible_v<Buffer>);
    Buffer first{3}; first.at(0) = 42;
    Buffer second{std::move(first)};
    CHECK(first.size() == 0 && second.at(0) == 42);
    first = Buffer{1};
    CHECK(first.size() == 1 && first.at(0) == 0);
    first = std::move(second);
    CHECK(second.size() == 0 && first.at(0) == 42);
    std::vector<Buffer> buffers;
    buffers.push_back(std::move(first));
    buffers.emplace_back(100);
    CHECK(buffers.front().at(0) == 42);
    CHECK(first.size() == 0);
    course::report();
}
