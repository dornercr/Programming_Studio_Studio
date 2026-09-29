#include "harbor/check.hpp"
#include <coroutine>
#include <exception>
#include <string>
#include <utility>
#include <iostream>
class StepTask {
public:
    struct promise_type {
        std::string value;
        std::exception_ptr error;
        StepTask get_return_object() { return StepTask{Handle::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::string s) { value=std::move(s); return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { error=std::current_exception(); }
    };
    using Handle=std::coroutine_handle<promise_type>;
private:
    Handle handle_;
public:
    explicit StepTask(Handle h):handle_(h) {}
    ~StepTask() { if (handle_) handle_.destroy(); }
    StepTask(const StepTask&)=delete;
    StepTask(StepTask&& other) noexcept:handle_(std::exchange(other.handle_,{})) {}
    bool next() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        if (handle_.promise().error) std::rethrow_exception(handle_.promise().error);
        return !handle_.done();
    }
    const auto& value() const { return handle_.promise().value; }
};
StepTask stages(std::string owned_key) {
    co_yield "validated:"+owned_key;
    co_yield "scheduled:"+owned_key;
}
int main() {
    auto task=stages(std::string("job-1"));
    harbor::check(task.next() && task.value()=="validated:job-1","first suspension");
    harbor::check(task.next() && task.value()=="scheduled:job-1","owned across suspension");
    harbor::check(!task.next(),"completion");
    std::cout<<"Coroutine lifetime demonstrated; no I/O scheduler is implied.\n";
}
