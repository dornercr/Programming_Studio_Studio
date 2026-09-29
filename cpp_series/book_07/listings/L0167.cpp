#include <future>
#include <optional>
#include <iostream>

int main() {
    std::promise<int> producer;auto shared=producer.get_future().share();unsigned authority_calls=0;
    std::optional<std::shared_future<int>> in_flight;
    auto request=[&](){if(!in_flight){++authority_calls;in_flight=shared;}return *in_flight;};
    auto first=request(),second=request();producer.set_value(42);
    std::cout<<"first="<<first.get()<<" second="<<second.get()<<" authority-calls="<<authority_calls<<'\n';
}
