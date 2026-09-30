#include "relay.hpp"
#include "linux_support.hpp"
#include "bounded_queue.hpp"
#include <atomic>
#include <charconv>
#include <exception>
#include <sstream>
#include <thread>
#include <vector>

namespace harbor {
std::string process_request(std::string_view request) {
    if (request.size() > 4096) return "ERR size";
    std::istringstream input{std::string(request)};
    std::string token;
    if (!(input >> token) || token != "SUM") return "ERR command";
    long long sum = 0;
    std::size_t count = 0;
    while (input >> token) {
        if (++count > 64) return "ERR count";
        int value{};
        const auto [end, error] = std::from_chars(token.data(), token.data()+token.size(), value);
        if (error != std::errc{} || end != token.data()+token.size() ||
            value < -1000000 || value > 1000000) return "ERR value";
        sum += value;
    }
    if (count == 0) return "ERR empty";
    return "OK " + std::to_string(sum);
}
RelayStats serve_n(int listener, std::size_t connections) {
    using harbor_linux::Fd;
    if (connections == 0 || connections > 1000)
        throw std::invalid_argument("connection count must be 1..1000");
    BoundedQueue<Fd> work{4};
    std::atomic<std::size_t> completed{}, failed{};
    std::vector<std::jthread> workers;
    struct CloseFirst {
        BoundedQueue<Fd>& queue;
        ~CloseFirst() { queue.close(); }
    } guard{work};
    for (int id = 0; id < 2; ++id) {
        workers.emplace_back([&] {
            while (auto connection = work.pop()) {
                try {
                    auto request = harbor_linux::receive_frame(connection->get());
                    if (!request) throw std::runtime_error("missing request");
                    const auto response = process_request(*request);
                    harbor_linux::send_frame(connection->get(), response);
                    completed.fetch_add(1, std::memory_order_relaxed);
                } catch (...) {
                    failed.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }
    std::exception_ptr accept_error;
    try {
        for (std::size_t i = 0; i < connections; ++i)
            if (!work.push(harbor_linux::accept_one(listener)))
                throw std::runtime_error("queue unexpectedly closed");
    } catch (...) { accept_error = std::current_exception(); }
    work.close();
    workers.clear();
    if (accept_error) std::rethrow_exception(accept_error);
    return {completed.load(), failed.load()};
}
}
