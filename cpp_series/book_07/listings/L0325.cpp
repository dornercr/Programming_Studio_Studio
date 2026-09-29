#include "harbor/net.hpp"
#include "harbor/store.hpp"
#include <atomic>
#include <csignal>
#include <iostream>
#include <sstream>
namespace {
static_assert(std::atomic<bool>::is_always_lock_free,
              "signal notification requires a lock-free atomic bool");
constinit std::atomic<bool> stop_requested{false};
extern "C" void request_stop(int) noexcept {
    stop_requested.store(true,std::memory_order_relaxed);
}
}
int main(int argc,char** argv) {
    try {
        if (argc<3 || argc>4) throw std::invalid_argument("usage: harbor_worker DATABASE PORT [--drop-first-reply]");
        bool drop=argc==4;
        if (drop && std::string(argv[3])!="--drop-first-reply") throw std::invalid_argument("unknown option");
        harbor::Store store(argv[1]);
        harbor::Server server(harbor::port_number(argv[2],true));
        if (std::signal(SIGINT,request_stop)==SIG_ERR ||
            std::signal(SIGTERM,request_stop)==SIG_ERR)
            throw std::runtime_error("install signal handler");
        std::cout<<"READY "<<server.port()<<std::endl;
        server.run([&](std::string_view message)->std::optional<std::string> {
            if (message=="PING") return "PONG";
            if (message=="COUNT") return std::to_string(store.receipt_count());
            std::istringstream in{std::string(message)};
            std::string verb,key,input,extra;
            if (!(in>>verb>>key>>input) || (in>>extra) || verb!="EXEC") return "INVALID";
            auto parsed=harbor::parse_request(key,input);
            auto* request=std::get_if<harbor::Request>(&parsed);
            if (!request) return "INVALID";
            try {
                auto result=store.apply(*request);
                if (drop) { drop=false; return {}; } // deliberate lost acknowledgment AFTER commit
                return "RESULT "+key+" "+std::to_string(result);
            } catch (const std::exception&) { return "ERROR"; }
        },[]{ return stop_requested.load(std::memory_order_relaxed); });
        return 0;
    } catch (const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
