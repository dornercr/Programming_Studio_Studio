#include "check.hpp"
#include "relay.hpp"
#include "linux_support.hpp"
#include <array>
#include <charconv>
#include <chrono>
#include <exception>
#include <string_view>
#include <thread>

unsigned number(std::string_view text, unsigned maximum) {
    unsigned result{};
    auto [end, error] = std::from_chars(text.data(), text.data()+text.size(), result);
    if (error != std::errc{} || end != text.data()+text.size() || result == 0 || result > maximum)
        throw std::invalid_argument("numeric argument out of range");
    return result;
}
std::string request(unsigned port, std::string_view message) {
    auto connection = harbor_linux::connect_loopback(static_cast<std::uint16_t>(port));
    harbor_linux::send_frame(connection.get(), message);
    auto response = harbor_linux::receive_frame(connection.get());
    if (!response) throw std::runtime_error("missing response");
    return *response;
}
int main(int argc, char** argv) {
    try {
        if (argc == 3 && std::string_view(argv[1]) == "--serve") {
            const auto count = number(argv[2], 1000);
            auto listener = harbor_linux::listen_loopback();
            std::cout << "127.0.0.1:" << listener.port << std::endl;
            auto stats = harbor::serve_n(listener.socket.get(), count);
            std::cout << "completed=" << stats.completed << " failed=" << stats.failed << '\n';
            return stats.failed == 0 ? 0 : 1;
        }
        if (argc == 4 && std::string_view(argv[1]) == "--request") {
            std::cout << request(number(argv[2], 65535), argv[3]) << '\n';
            return 0;
        }
        if (argc != 1) { std::cerr << "usage: relay [--serve COUNT | --request PORT TEXT]\n"; return 2; }
        CHECK(harbor::process_request("SUM 2 -1 4") == "OK 5");
        CHECK(harbor::process_request("SUM") == "ERR empty");
        CHECK(harbor::process_request("SUM 4x") == "ERR value");
        auto listener = harbor_linux::listen_loopback();
        harbor::RelayStats stats;
        std::exception_ptr server_error;
        const auto start = std::chrono::steady_clock::now();
        std::jthread server([&] {
            try { stats = harbor::serve_n(listener.socket.get(), 4); }
            catch (...) { server_error = std::current_exception(); }
        });
        const std::array<std::string_view, 4> messages{"SUM 2 3", "BAD", "SUM -7 2", "SUM 1000001"};
        const std::array<std::string_view, 4> expected{"OK 5", "ERR command", "OK -5", "ERR value"};
        for (std::size_t i = 0; i < messages.size(); ++i) CHECK(request(listener.port, messages[i]) == expected[i]);
        server.join();
        if (server_error) std::rethrow_exception(server_error);
        CHECK(stats.completed == 4 && stats.failed == 0);
        const auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now()-start).count();
        std::cout << "completed=" << stats.completed << " elapsed_ms=" << elapsed << "\nPASS\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
