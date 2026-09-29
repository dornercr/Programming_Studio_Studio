#include "harbor/check.hpp"
#include "harbor/net.hpp"
#include <thread>
#include <iostream>
int main() {
    harbor::Server server(0);
    std::jthread loop([&](std::stop_token stop) {
        server.run([](std::string_view s)->std::optional<std::string> {
            return "echo:"+std::string(s);
        },[&]{return stop.stop_requested();});
    });
    auto reply=harbor::exchange(server.port(),"alpha",std::chrono::milliseconds(1000));
    harbor::check(reply=="echo:alpha","loopback exchange");
    loop.request_stop();
    std::cout<<"Nonblocking loopback request completed through a bounded frame.\n";
}
