#include <cassert>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <numeric>
#include <optional>
#include <vector>
#include <condition_variable>
#include <deque>
#include <future>
#include <stdexcept>

class Queue {
    std::mutex mutex_;
    std::condition_variable readable_, writable_;
    std::deque<int> items_;
    std::size_t capacity_;
    bool closed_ = false;
public:
    explicit Queue(std::size_t capacity) : capacity_(capacity) {
        if(capacity == 0) throw std::invalid_argument("zero capacity");
    }
    bool push(int value) {
        std::unique_lock<std::mutex> lock(mutex_);
        writable_.wait(lock,[&] { return closed_ || items_.size() < capacity_; });
        if(closed_) return false;
        items_.push_back(value);
        readable_.notify_one();
        return true;
    }
    std::optional<int> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        readable_.wait(lock,[&] { return closed_ || !items_.empty(); });
        if(items_.empty()) return std::nullopt;
        const int value = items_.front(); items_.pop_front();
        writable_.notify_one();
        return value;
    }
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        readable_.notify_all(); writable_.notify_all();
    }
};

int main() {
    Queue queue(2);
    std::vector<int> received;
    received.reserve(5); // Allocate before a producer can block.
    auto producer = std::async(std::launch::async,[&] {
        try {
            for(int i=1;i<=5;++i) assert(queue.push(i));
            queue.close();
        } catch(...) { queue.close(); throw; }
    });
    try {
        while(const auto item = queue.pop()) received.push_back(*item);
    } catch(...) {
        queue.close();
        try { producer.get(); } catch(...) {}
        throw;
    }
    producer.get();
    assert((received == std::vector<int>{1,2,3,4,5}));
    assert(!queue.push(6) && !queue.pop());
    bool rejected = false;
    try { Queue invalid(0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "items=" << received.size() << " sum="
              << std::accumulate(received.begin(),received.end(),0) << " closed=yes\n";
}
