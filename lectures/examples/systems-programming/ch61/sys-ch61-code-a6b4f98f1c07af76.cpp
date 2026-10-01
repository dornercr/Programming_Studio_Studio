#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <condition_variable>
#include <deque>
#include <future>
#include <stdexcept>

struct Work { std::size_t index; std::string bytes; };
struct Result { bool accepted; std::size_t bytes; std::uint64_t sum; };
class WorkQueue {
    std::mutex mutex_;
    std::condition_variable readable_, writable_;
    std::deque<Work> work_;
    const std::size_t capacity_;
    bool closed_ = false;
public:
    explicit WorkQueue(std::size_t capacity) : capacity_(capacity) {
        if(capacity == 0) throw std::invalid_argument("capacity");
    }
    bool push(Work work) {
        std::unique_lock<std::mutex> lock(mutex_);
        writable_.wait(lock,[&] { return closed_ || work_.size() < capacity_; });
        if(closed_) return false;
        work_.push_back(std::move(work));
        readable_.notify_one();
        return true;
    }
    std::optional<Work> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        readable_.wait(lock,[&] { return closed_ || !work_.empty(); });
        if(work_.empty()) return std::nullopt;
        Work result = std::move(work_.front()); work_.pop_front();
        writable_.notify_one();
        return result;
    }
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true; readable_.notify_all(); writable_.notify_all();
    }
};
Result process(const std::string& bytes) {
    if(bytes.size() > 4) return {false,0,0};
    std::uint64_t sum = 0;
    for(unsigned char byte : bytes) sum += byte;
    return {true,bytes.size(),sum};
}
std::vector<std::optional<Result>> pipeline(const std::vector<std::string>& inputs) {
    WorkQueue queue(2);
    std::vector<std::optional<Result>> results(inputs.size());
    std::vector<std::future<void>> workers;
    workers.reserve(2); // Allocate before any worker can wait on the queue.
    const auto worker = [&] {
        try {
            while(auto work = queue.pop()) results[work->index] = process(work->bytes);
        } catch(...) { queue.close(); throw; }
    };
    try {
        workers.push_back(std::async(std::launch::async,worker));
        workers.push_back(std::async(std::launch::async,worker));
        for(std::size_t i=0;i<inputs.size();++i)
            if(!queue.push({i,inputs[i]})) throw std::runtime_error("queue closed early");
        queue.close();
        for(auto& future : workers) future.get();
    } catch(...) {
        queue.close();
        for(auto& future : workers) if(future.valid()) {
            try { future.get(); } catch(...) {} // Preserve the original failure.
        }
        throw;
    }
    return results;
}

static_assert('A' == 65 && 'c' == 99 && 'a' == 97 && 't' == 116);
int main() {
    const std::vector<std::string> inputs{"cat","A","","tools"};
    const auto results = pipeline(inputs);
    const std::array<Result,4> expected{{{true,3,312},{true,1,65},{true,0,0},{false,0,0}}};
    std::size_t accepted = 0, rejected = 0, bytes = 0;
    std::uint64_t sum = 0;
    for(std::size_t i=0;i<results.size();++i) {
        assert(results[i]);
        const auto result = *results[i];
        assert(result.accepted == expected[i].accepted && result.bytes == expected[i].bytes
               && result.sum == expected[i].sum);
        std::cout << i << " accepted=" << result.accepted << " bytes=" << result.bytes
                  << " sum=" << result.sum << '\n';
        if(result.accepted) { ++accepted; bytes += result.bytes; sum += result.sum; }
        else ++rejected;
    }
    assert(accepted == 3 && rejected == 1 && bytes == 4 && sum == 377);
    assert(pipeline({}).empty());
    std::cout << "accepted=" << accepted << " rejected=" << rejected
              << " bytes=" << bytes << " sum=" << sum << '\n';
}
