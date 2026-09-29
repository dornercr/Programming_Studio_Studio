#pragma once
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace harbor {
// Element-count bounded, not allocation-free. try_push consumes its value
// argument even on rejection; resource-owning callers must account for that.
template<class T> class BoundedInbox {
    static_assert(std::is_nothrow_move_constructible_v<T>);
    std::mutex mutex_;
    std::condition_variable changed_;
    std::deque<T> values_;
    std::size_t capacity_;
    bool closed_ = false;
public:
    explicit BoundedInbox(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) throw std::invalid_argument("zero inbox capacity");
    }
    bool try_push(T value) {
        {
            std::lock_guard lock(mutex_);
            if (closed_ || values_.size() == capacity_) return false;
            values_.push_back(std::move(value));
        }
        changed_.notify_one();
        return true;
    }
    std::optional<T> pop() {
        std::unique_lock lock(mutex_);
        changed_.wait(lock, [&] { return closed_ || !values_.empty(); });
        if (values_.empty()) return std::nullopt;
        T value = std::move(values_.front());
        values_.pop_front();
        return value;
    }
    void close() {
        { std::lock_guard lock(mutex_); closed_ = true; }
        changed_.notify_all();
    }
};
} // namespace harbor
