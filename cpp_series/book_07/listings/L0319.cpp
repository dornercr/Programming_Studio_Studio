#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <utility>
namespace harbor {
template<class T> class BoundedQueue {
    const std::size_t capacity_;
    std::deque<T> items_;
    bool closed_ = false;
    mutable std::mutex mutex_;
    std::condition_variable ready_;
public:
    explicit BoundedQueue(std::size_t n): capacity_(n) {
        if (!n) throw std::invalid_argument("queue capacity");
    }
    bool try_push(T value) {
        std::lock_guard lock(mutex_);
        if (closed_ || items_.size() >= capacity_) return false;
        items_.push_back(std::move(value)); ready_.notify_one(); return true;
    }
    std::optional<T> pop() {
        std::unique_lock lock(mutex_);
        ready_.wait(lock, [&]{ return closed_ || !items_.empty(); });
        if (items_.empty()) return {};
        T value = std::move(items_.front()); items_.pop_front(); return value;
    }
    void close() { std::lock_guard lock(mutex_); closed_ = true; ready_.notify_all(); }
    std::size_t size() const { std::lock_guard lock(mutex_); return items_.size(); }
};
}
