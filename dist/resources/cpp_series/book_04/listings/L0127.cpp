#include "relay.hpp"
#include <charconv>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <thread>

namespace {
unsigned number(std::string_view text, unsigned maximum) {
    unsigned value = 0;
    auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc{} || end != text.data() + text.size() ||
        value == 0 || value > maximum) throw std::invalid_argument("invalid numeric argument");
    return value;
}
void print_stats(harbor::Stats s) {
    std::cout << "accepted=" << s.accepted << " admitted=" << s.admitted
              << " rejected=" << s.rejected << " completed=" << s.completed
              << " app_errors=" << s.application_errors << " failures=" << s.failures
              << " timeouts=" << s.timeouts << " cancelled=" << s.cancelled << '\n';
}
}
int main(int argc, char** argv) {
    try {
        if (argc == 1 || (argc == 2 && std::string_view(argv[1]) == "--self-test")) {
            harbor::RelayServer server;
            if (harbor::request(server.port(), "SUM 7 -2 9") != "OK 14" ||
                harbor::request(server.port(), "SUM nope") != "ERR value")
                throw std::runtime_error("self-test response mismatch");
            server.stop_and_join(); server.rethrow_failure();
            print_stats(server.snapshot());
            std::cout << "PASS\n";
            return 0;
        }
        if (argc == 3 && std::string_view(argv[1]) == "--serve") {
            const auto seconds = number(argv[2], 3600);
            harbor::RelayServer server;
            std::cout << "127.0.0.1:" << server.port() << '\n' << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
            server.stop_and_join(); server.rethrow_failure();
            print_stats(server.snapshot());
            return 0;
        }
        if (argc == 4 && std::string_view(argv[1]) == "--request") {
            std::cout << harbor::request(number(argv[2], 65535), argv[3]) << '\n';
            return 0;
        }
        std::cerr << "usage: harbor_relay [--self-test | --serve SECONDS | --request PORT TEXT]\n";
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }
}
