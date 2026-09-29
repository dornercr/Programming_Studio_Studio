#include <future>
#include <string>
#include <iostream>
#include <thread>

int main() {
    std::promise<std::size_t> promise;auto result=promise.get_future();
    std::jthread worker([input=std::string("owned request"),p=std::move(promise)]()mutable{p.set_value(input.size());});
    std::cout<<"submitted\n";
    auto length=result.get();worker.join();std::cout<<"completed bytes="<<length<<'\n';
}
