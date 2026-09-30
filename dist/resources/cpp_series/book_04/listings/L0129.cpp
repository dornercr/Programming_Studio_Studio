#include "relay.hpp"
#include "frame.hpp"
#include "check.hpp"
#include <array>
#include <cerrno>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <span>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <vector>

using namespace std::chrono_literals;
namespace {
std::pair<harbor::Fd, harbor::Fd> pair_sockets() {
    int descriptors[2];
    if (::socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,
                     0, descriptors) < 0) throw std::runtime_error("socketpair");
    return {harbor::Fd(descriptors[0]), harbor::Fd(descriptors[1])};
}
harbor::Operation operation(std::chrono::milliseconds budget = 2s) {
    return {harbor::Clock::now() + budget};
}
template<class F> void transport_throws(F action, harbor::Failure expected) {
    bool matched = false;
    try { action(); }
    catch (const harbor::TransportError& e) { matched = e.category == expected; }
    CHECK(matched);
}
template<class Exception, class F> void throws(F action) {
    bool caught = false;
    try { action(); } catch (const Exception&) { caught = true; }
    CHECK(caught);
}
void send_raw(int fd, std::span<const std::byte> bytes) {
    harbor::write_exact(fd, bytes, operation());
}
void grammar() {
    using harbor::process_request;
    CHECK(process_request("SUM 1 -2 3") == "OK 2");
    CHECK(process_request("SUM") == "ERR empty");
    CHECK(process_request("") == "ERR command");
    CHECK(process_request("sum 1") == "ERR command");
    for (auto value : {"1x", "+1", "1000001", "-1000001", "9999999999999999"})
        CHECK(process_request(std::string("SUM ") + value) == "ERR value");
    CHECK(process_request("SUM -1000000 1000000") == "OK 0");
    std::string many = "SUM";
    for (int i = 0; i < 64; ++i) many += " 1";
    CHECK(process_request(many) == "OK 64");
    CHECK(process_request(many + " 1") == "ERR count");
    CHECK(process_request(std::string(4097, 'x')) == "ERR size");
    CHECK(process_request(std::string("SUM 1\0x", 7)) == "ERR value");
}
void codec_golden() {
    const std::array<std::byte, 7> golden{std::byte{0}, std::byte{0}, std::byte{0},
        std::byte{3}, std::byte{'A'}, std::byte{0}, std::byte{'B'}};
    const std::string text("A\0B", 3);
    CHECK(harbor::encode_frame(text) == std::vector<std::byte>(golden.begin(), golden.end()));
    harbor::FrameDecoder decoder;
    CHECK(decoder.feed(golden) == golden.size());
    CHECK(decoder.finish()); CHECK(decoder.payload() == text);
}
void codec_splits() {
    for (const auto& text : {std::string{}, std::string("SUM 1 2"), std::string("A\0B", 3)}) {
        const auto wire = harbor::encode_frame(text);
        for (std::size_t split = 0; split <= wire.size(); ++split) {
            harbor::FrameDecoder decoder;
            CHECK(decoder.feed(std::span(wire).first(split)) == split);
            CHECK(decoder.feed(std::span(wire).subspan(split)) == wire.size() - split);
            CHECK(decoder.finish()); CHECK(decoder.payload() == text);
        }
    }
    auto wire = harbor::encode_frame("one");
    auto second = harbor::encode_frame("two");
    wire.insert(wire.end(), second.begin(), second.end());
    harbor::FrameDecoder decoder;
    CHECK(decoder.feed(wire) == 7); CHECK(decoder.payload() == "one");
}
void codec_limits() {
    std::string payload(4096, 'x');
    auto wire = harbor::encode_frame(payload);
    harbor::FrameDecoder decoder;
    for (std::size_t i = 0; i < wire.size(); ++i)
        CHECK(decoder.feed(std::span(wire).subspan(i, 1)) == 1);
    CHECK(decoder.payload() == payload);
    throws<std::length_error>([] { (void)harbor::encode_frame(std::string(4097, 'x')); });
    std::array<std::byte, 4> bad{std::byte{0}, std::byte{0}, std::byte{16}, std::byte{1}};
    harbor::FrameDecoder invalid;
    throws<std::length_error>([&] { invalid.feed(bad); });
    throws<std::logic_error>([&] { invalid.feed({}); });
}
void codec_eof() {
    auto wire = harbor::encode_frame("abc");
    for (std::size_t n = 0; n < wire.size(); ++n) {
        harbor::FrameDecoder d;
        d.feed(std::span(wire).first(n));
        if (n == 0) CHECK(!d.finish());
        else throws<std::runtime_error>([&] { d.finish(); });
    }
}
void native_roundtrip() {
    auto [a, b] = pair_sockets();
    const std::string text("SUM 7\0 9", 8);
    harbor::send_frame(a.get(), text, operation());
    auto got = harbor::receive_frame(b.get(), operation());
    CHECK(got && *got == text);
}
void native_empty() {
    auto [a, b] = pair_sockets();
    harbor::send_frame(a.get(), "", operation());
    auto got = harbor::receive_frame(b.get(), operation());
    CHECK(got && got->empty());
    a.reset();
    CHECK(!harbor::receive_frame(b.get(), operation()));
}
void native_truncated_header() {
    auto [a, b] = pair_sockets();
    std::array<std::byte, 2> part{};
    send_raw(a.get(), part); a.reset();
    transport_throws([&] { harbor::receive_frame(b.get(), operation()); }, harbor::Failure::truncated);
}
void native_truncated_body() {
    auto [a, b] = pair_sockets();
    auto wire = harbor::encode_frame("hello");
    send_raw(a.get(), std::span(wire).first(6)); a.reset();
    transport_throws([&] { harbor::receive_frame(b.get(), operation()); }, harbor::Failure::truncated);
}
void native_missing_body() {
    auto [a, b] = pair_sockets();
    auto wire = harbor::encode_frame("hello");
    send_raw(a.get(), std::span(wire).first(4)); a.reset();
    transport_throws([&] { harbor::receive_frame(b.get(), operation()); }, harbor::Failure::truncated);
}
void native_oversize() {
    auto [a, b] = pair_sockets();
    std::array<std::byte, 4> bad{std::byte{255}, std::byte{255}, std::byte{255}, std::byte{255}};
    send_raw(a.get(), bad);
    transport_throws([&] { harbor::receive_frame(b.get(), operation()); }, harbor::Failure::oversize);
}
void native_deadline() {
    auto [a, b] = pair_sockets();
    transport_throws([&] { harbor::receive_frame(b.get(), operation(60ms)); }, harbor::Failure::timeout);
    // A previously expired absolute deadline cannot be reset by available data.
    harbor::send_frame(a.get(), "ready", operation());
    harbor::Operation expired{harbor::Clock::now() - 1ms};
    transport_throws([&] { harbor::receive_frame(b.get(), expired); }, harbor::Failure::timeout);
}
void native_cancel() {
    auto [a, b] = pair_sockets();
    std::atomic<bool> stop{true};
    auto op = operation(); op.stop = &stop;
    transport_throws([&] { harbor::receive_frame(b.get(), op); }, harbor::Failure::cancelled);
    transport_throws([&] { harbor::send_frame(a.get(), "x", op); }, harbor::Failure::cancelled);
}
void native_fragmented() {
    auto [a, b] = pair_sockets();
    auto wire = harbor::encode_frame("fragmented data");
    std::exception_ptr error;
    std::jthread writer([&] {
        try { for (std::size_t i = 0; i < wire.size(); ++i)
            send_raw(a.get(), std::span(wire).subspan(i, 1)); }
        catch (...) { error = std::current_exception(); }
    });
    auto got = harbor::receive_frame(b.get(), operation());
    writer.join(); if (error) std::rethrow_exception(error);
    CHECK(got && *got == "fragmented data");
}
void inbox_state() {
    harbor::BoundedInbox<std::unique_ptr<int>> q(1);
    CHECK(q.try_push(std::make_unique<int>(7)));
    CHECK(!q.try_push(std::make_unique<int>(8)));
    q.close(); q.close();
    CHECK(!q.try_push(std::make_unique<int>(9)));
    auto item = q.pop(); CHECK(item && **item == 7); CHECK(!q.pop());
    throws<std::invalid_argument>([] { harbor::BoundedInbox<int> bad(0); });
}
void accounting(harbor::Stats s) {
    CHECK(s.accepted == s.admitted + s.rejected);
    CHECK(s.admitted == s.completed + s.failures + s.cancelled);
    CHECK(s.timeouts <= s.failures); CHECK(s.application_errors <= s.completed);
}
void wait_completed(harbor::RelayServer& server, std::uint64_t expected) {
    const auto until = harbor::Clock::now() + 5s;
    while (server.snapshot().completed < expected) {
        const auto s = server.snapshot();
        if (s.failures || s.cancelled)
            throw std::runtime_error("unexpected server failure before quiescence");
        if (harbor::Clock::now() >= until)
            throw std::runtime_error("server completion observation deadline");
        std::this_thread::yield();
    }
}
void server_smoke() {
    harbor::RelayServer server;
    CHECK(harbor::request(server.port(), "SUM 8 -3") == "OK 5");
    CHECK(harbor::request(server.port(), "SUM bad") == "ERR value");
    wait_completed(server, 2); // receipt can precede worker bookkeeping
    server.stop_and_join(); server.rethrow_failure();
    auto s = server.snapshot(); accounting(s);
    CHECK(s.completed == 2); CHECK(s.application_errors == 1);
}
void server_parallel() {
    harbor::Config config; config.workers = 4; config.queue_capacity = 16;
    config.request_budget = 5s;
    harbor::RelayServer server(config);
    std::array<std::string, 8> results;
    std::array<std::exception_ptr, 8> errors;
    std::vector<std::jthread> clients;
    for (std::size_t i = 0; i < results.size(); ++i) clients.emplace_back([&, i] {
        try { results[i] = harbor::request(server.port(), "SUM " + std::to_string(i) + " 10"); }
        catch (...) { errors[i] = std::current_exception(); }
    });
    clients.clear();
    for (std::size_t i = 0; i < results.size(); ++i) {
        if (errors[i]) std::rethrow_exception(errors[i]);
        CHECK(results[i] == "OK " + std::to_string(i + 10));
    }
    wait_completed(server, 8);
    server.stop_and_join(); server.rethrow_failure(); accounting(server.snapshot());
    CHECK(server.snapshot().completed == 8);
}
void wait_accepted(harbor::RelayServer& server) {
    const auto until = harbor::Clock::now() + 3s;
    while (server.snapshot().accepted == 0) {
        if (harbor::Clock::now() >= until) throw std::runtime_error("accept observation deadline");
        std::this_thread::yield();
    }
}
void server_idle_stop() {
    harbor::RelayServer server;
    auto socket = harbor::connect_local(server.port(), operation());
    wait_accepted(server);
    server.stop_and_join(); server.rethrow_failure(); accounting(server.snapshot());
    CHECK(server.snapshot().completed == 0);
}
void server_no_clients() {
    harbor::RelayServer server;
    server.stop_and_join(); server.stop_and_join(); server.rethrow_failure();
    accounting(server.snapshot()); CHECK(server.snapshot().accepted == 0);
}
void server_timeout() {
    harbor::Config config; config.request_budget = 60ms;
    harbor::RelayServer server(config);
    auto socket = harbor::connect_local(server.port(), operation());
    // Peer closure proves its owned operation finished; no millisecond speed assertion.
    auto response = harbor::receive_frame(socket.get(), operation(3s));
    CHECK(!response);
    server.stop_and_join(); server.rethrow_failure(); accounting(server.snapshot());
    CHECK(server.snapshot().timeouts == 1);
}
void server_response_stop_race() {
    harbor::RelayServer server;
    CHECK(harbor::request(server.port(), "SUM 3") == "OK 3");
    // Do not order server bookkeeping by a client-side receive. A final
    // stop/deadline check may classify cancellation after bytes were sent.
    server.stop_and_join(); server.rethrow_failure();
    const auto s = server.snapshot(); accounting(s);
    CHECK(s.accepted == 1); CHECK(s.admitted == 1);
    CHECK(s.completed + s.failures + s.cancelled == 1);
}
void server_start_failure() {
    for (unsigned fail = 0; fail < 2; ++fail) {
        harbor::Config config; config.fail_before_worker = fail;
        throws<std::runtime_error>([&] { harbor::RelayServer server(config); });
    }
    harbor::RelayServer healthy;
    CHECK(harbor::request(healthy.port(), "SUM 1") == "OK 1");
    healthy.stop_and_join(); healthy.rethrow_failure();
}
void invalid_config() {
    for (unsigned workers : {0u, 17u}) {
        harbor::Config c; c.workers = workers;
        throws<std::invalid_argument>([&] { harbor::RelayServer s(c); });
    }
    for (auto capacity : {std::size_t{0}, std::size_t{65}}) {
        harbor::Config c; c.queue_capacity = capacity;
        throws<std::invalid_argument>([&] { harbor::RelayServer s(c); });
    }
    throws<std::invalid_argument>([] { harbor::connect_local(0, operation()); });
}
}
int main(int argc, char** argv) {
    const std::map<std::string, std::function<void()>> cases{
        {"grammar", grammar}, {"codec_golden", codec_golden}, {"codec_splits", codec_splits},
        {"codec_limits", codec_limits}, {"codec_eof", codec_eof},
        {"native_roundtrip", native_roundtrip}, {"native_empty", native_empty},
        {"native_truncated_header", native_truncated_header},
        {"native_truncated_body", native_truncated_body}, {"native_missing_body", native_missing_body},
        {"native_oversize", native_oversize}, {"native_deadline", native_deadline},
        {"native_cancel", native_cancel}, {"native_fragmented", native_fragmented},
        {"inbox_state", inbox_state}, {"server_smoke", server_smoke},
        {"server_parallel", server_parallel}, {"server_idle_stop", server_idle_stop},
        {"server_no_clients", server_no_clients}, {"server_timeout", server_timeout},
        {"server_start_failure", server_start_failure},
        {"server_response_stop_race", server_response_stop_race}, {"invalid_config", invalid_config}
    };
    try {
        if (argc != 2 || !cases.contains(argv[1])) throw std::invalid_argument("unknown test case");
        cases.at(argv[1])();
        std::cout << "PASS " << argv[1] << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n'; return 1;
    }
}
