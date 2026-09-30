#include <coroutine>
#include <string>
#include <iostream>
#include <utility>
#include <exception>

struct Task{
    struct promise_type{
        Task get_return_object(){return Task{std::coroutine_handle<promise_type>::from_promise(*this)};}
        std::suspend_always initial_suspend()noexcept{return{};}
        std::suspend_always final_suspend()noexcept{return{};}
        void return_void(){}void unhandled_exception(){std::terminate();}
    };
    std::coroutine_handle<promise_type> h;
    explicit Task(std::coroutine_handle<promise_type>x):h(x){}
    Task(const Task&)=delete;Task&operator=(const Task&)=delete;
    Task(Task&&x)noexcept:h(std::exchange(x.h,{})){}
    ~Task(){if(h)h.destroy();}
    void resume(){if(h&&!h.done())h.resume();}
};
Task make_task(std::string text){std::cout<<"body owns "<<text<<'\n';co_await std::suspend_always{};std::cout<<"resumed with "<<text<<'\n';}

int main() {
    auto task=make_task(std::string("retained"));
    std::cout<<"created without executing body\n";
    task.resume();
    std::cout<<"first resume reached suspension\n";
    task.resume();
}
