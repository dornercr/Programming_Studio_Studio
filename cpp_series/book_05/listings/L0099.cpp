#ifndef HARBOR_TASK_POOL_HPP
#define HARBOR_TASK_POOL_HPP
#include "bounded_queue.hpp"
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
class TaskPool {
    BoundedQueue<std::function<void()>> work_;
    std::vector<std::jthread> workers_;
public:
    explicit TaskPool(std::size_t workers,std::size_t capacity=8):work_(capacity) {
        if (workers==0 || workers>64) throw std::invalid_argument("worker count");
        try {
            for (std::size_t i=0;i<workers;++i)
                workers_.emplace_back([this] { while (auto task=work_.pop()) (*task)(); });
        } catch (...) { work_.close(); workers_.clear(); throw; }
    }
    TaskPool(const TaskPool&)=delete;
    TaskPool& operator=(const TaskPool&)=delete;
    ~TaskPool() { shutdown(); }
    std::future<long long> submit(std::function<long long()> operation) {
        if (!operation) throw std::invalid_argument("empty task");
        auto task=std::make_shared<std::packaged_task<long long()>>(std::move(operation));
        auto result=task->get_future();
        if (!work_.push([task] { (*task)(); })) throw std::runtime_error("pool closed");
        return result;
    }
    // Owner-thread only; never call from a worker or concurrently with shutdown.
    void shutdown() { work_.close(); workers_.clear(); }
};
#endif
