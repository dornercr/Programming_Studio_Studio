#ifndef HARBOR_SERIES_BOUNDED_QUEUE_HPP
#define HARBOR_SERIES_BOUNDED_QUEUE_HPP
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

template<class T>
class BoundedQueue {
    static_assert(std::is_nothrow_move_constructible_v<T>);
    std::mutex mutex_;
    std::condition_variable not_empty_, not_full_;
    std::deque<T> data_;
    std::size_t capacity_;
    bool closed_{};
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) throw std::invalid_argument("queue capacity");
    }
    BoundedQueue(const BoundedQueue&) = delete;
    BoundedQueue& operator=(const BoundedQueue&) = delete;
    bool push(T value) {
        std::unique_lock lock{mutex_};
        not_full_.wait(lock, [&] { return closed_ || data_.size() < capacity_; });
        if (closed_) return false;
        data_.push_back(std::move(value));
        lock.unlock();
        not_empty_.notify_one();
        return true;
    }
    std::optional<T> pop() {
        std::unique_lock lock{mutex_};
        not_empty_.wait(lock, [&] { return closed_ || !data_.empty(); });
        if (data_.empty()) return std::nullopt;
        T value = std::move(data_.front());
        data_.pop_front();
        lock.unlock();
        not_full_.notify_one();
        return value;
    }
    void close() {
        { std::lock_guard lock{mutex_}; closed_ = true; }
        not_empty_.notify_all();
        not_full_.notify_all();
    }
};
#endif
