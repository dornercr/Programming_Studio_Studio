#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

std::atomic_bool stopping{false};
extern "C" void request_stop(int) { stopping.store(true); }

int main() {
    std::signal(SIGTERM, request_stop);
    std::signal(SIGINT, request_stop);
    std::cout << "ready=1\n";
    while (!stopping.load()) std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "ready=0 draining=1\n";
    std::cout << "shutdown=complete\n";
}
