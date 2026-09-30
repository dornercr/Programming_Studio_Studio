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

#include <cstddef>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

class IntVector {
    std::unique_ptr<int[]> data_;
    std::size_t size_{}, capacity_{};
public:
    IntVector() = default;
    IntVector(const IntVector&) = delete;
    IntVector& operator=(const IntVector&) = delete;
    IntVector(IntVector&& other) noexcept
        : data_(std::move(other.data_)), size_(std::exchange(other.size_, 0)),
          capacity_(std::exchange(other.capacity_, 0)) {}
    IntVector& operator=(IntVector&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    void reserve(std::size_t requested) {
        if (requested <= capacity_) return;
        if (requested > std::numeric_limits<std::size_t>::max() / sizeof(int)) {
            throw std::length_error("capacity overflow");
        }
        auto replacement = std::make_unique<int[]>(requested);
        for (std::size_t i = 0; i < size_; ++i) replacement[i] = data_[i];
        data_.swap(replacement);
        capacity_ = requested;
    }
    void push_back(int value) {
        if (size_ == capacity_) {
            const auto maximum = std::numeric_limits<std::size_t>::max() / sizeof(int);
            if (capacity_ > maximum / 2) throw std::length_error("growth overflow");
            reserve(capacity_ ? capacity_ * 2 : 1);
        }
        data_[size_++] = value;
    }
    void pop_back() {
        if (size_ == 0) throw std::out_of_range("empty vector");
        --size_;
    }
    int& at(std::size_t index) {
        if (index >= size_) throw std::out_of_range("index");
        return data_[index];
    }
};

int main() {
    IntVector actual;
    std::vector<int> expected;
    CHECK(course::throws<std::out_of_range>([&] { actual.pop_back(); }));
    for (int i = 0; i < 1000; ++i) {
        actual.push_back(i); expected.push_back(i);
        CHECK(actual.size() == expected.size() && actual.capacity() >= actual.size());
    }
    for (std::size_t i = 0; i < expected.size(); ++i) CHECK(actual.at(i) == expected[i]);
    actual.push_back(actual.at(0)); // Parameter copied before any reallocation.
    CHECK(actual.at(1000) == 0);
    actual.pop_back();
    IntVector moved{std::move(actual)};
    CHECK(actual.size() == 0 && moved.size() == 1000);
    actual.push_back(42);
    CHECK(actual.at(0) == 42);
    CHECK(course::throws<std::out_of_range>([&] { (void)moved.at(1000); }));
    course::report();
}
