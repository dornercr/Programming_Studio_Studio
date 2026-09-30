#include "harbor/net.hpp"
#include "harbor/store.hpp"
#include "harbor/policies.hpp"
#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <thread>
namespace {
static_assert(std::atomic<bool>::is_always_lock_free,
              "signal notification requires a lock-free atomic bool");
constinit std::atomic<bool> stop_requested{false};
extern "C" void request_stop(int) noexcept {
    stop_requested.store(true,std::memory_order_relaxed);
}
struct Counters {
    std::atomic<std::uint64_t> accepted{0}, duplicate{0}, conflict{0}, dispatch_errors{0};
};
void pause_for(std::stop_token stop, std::chrono::milliseconds time) {
    auto until=harbor::Steady::now()+time;
    while (!stop.stop_requested() && harbor::Steady::now()<until)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
}
int main(int argc,char** argv) {
    try {
        if (argc!=4) throw std::invalid_argument("usage: harbor_gateway DATABASE PORT WORKER_PORT");
        auto worker_port=harbor::port_number(argv[3]);
        harbor::Store api_store(argv[1]);
        auto dispatch_store=std::make_unique<harbor::Store>(argv[1]);
        harbor::Server server(harbor::port_number(argv[2],true));
        Counters counters;
        const auto random_seed=std::random_device{}(); // failure stays in main startup catch
        if (std::signal(SIGINT,request_stop)==SIG_ERR ||
            std::signal(SIGTERM,request_stop)==SIG_ERR)
            throw std::runtime_error("install signal handler");
        std::jthread dispatcher([&,db=std::move(dispatch_store),random_seed](std::stop_token stop) {
            harbor::RetryPolicy retry; unsigned failures=0;
            std::mt19937 rng(random_seed);
            while (!stop.stop_requested()) {
                bool failed=false;
                try {
                    for (const auto& request : db->pending()) {
                        if (stop.stop_requested()) break;
                        auto reply=harbor::exchange(worker_port,"EXEC "+request.key+" "+
                            std::to_string(request.input),std::chrono::milliseconds(200));
                        std::istringstream in(reply); std::string verb,key,extra; std::int64_t value{};
                        if (!(in>>verb>>key>>value) || (in>>extra) || verb!="RESULT" || key!=request.key)
                            throw std::runtime_error("worker reply contract");
                        db->complete(request,value); failures=0;
                    }
                } catch (const std::exception&) {
                    ++counters.dispatch_errors; failed=true;
                    failures=std::min(failures+1,10U);
                }
                // One local delivery loop owns retry. Pending rows survive restarts.
                auto wait=failed ? std::max<harbor::Tick>(10,retry.delay(failures-1,rng)) : 25;
                pause_for(stop,std::chrono::milliseconds(wait));
            }
        });
        std::cout<<"READY "<<server.port()<<std::endl;
        server.run([&](std::string_view message)->std::optional<std::string> {
            if (message=="PING") return "PONG";
            if (message=="METRICS") {
                return "harbor_accepted_total "+std::to_string(counters.accepted.load())+"\n"+
                    "harbor_duplicate_total "+std::to_string(counters.duplicate.load())+"\n"+
                    "harbor_conflict_total "+std::to_string(counters.conflict.load())+"\n"+
                    "harbor_dispatch_errors_total "+std::to_string(counters.dispatch_errors.load())+"\n";
            }
            std::istringstream in{std::string(message)}; std::string verb,key,input,extra;
            if (!(in>>verb>>key) || !harbor::valid_key(key)) return "INVALID";
            try {
                if (verb=="STATUS") {
                    if (in>>extra) return "INVALID";
                    auto job=api_store.lookup(key);
                    if (!job) return "MISSING";
                    if (!job->result) return "PENDING "+key;
                    return "DONE "+key+" "+std::to_string(*job->result);
                }
                if (verb!="SUBMIT" || !(in>>input) || (in>>extra)) return "INVALID";
                auto parsed=harbor::parse_request(key,input);
                auto* request=std::get_if<harbor::Request>(&parsed);
                if (!request) return "INVALID";
                switch (api_store.submit(*request)) {
                    case harbor::Store::Submit::accepted: ++counters.accepted; break;
                    case harbor::Store::Submit::duplicate: ++counters.duplicate; break;
                    case harbor::Store::Submit::conflict: ++counters.conflict; return "CONFLICT";
                    case harbor::Store::Submit::full: return "BUSY";
                }
                return "ACCEPTED "+key;
            } catch (const std::exception&) { return "ERROR"; }
        },[]{ return stop_requested.load(std::memory_order_relaxed); });
        dispatcher.request_stop(); // jthread joins before counters/stores are destroyed
        return 0;
    } catch (const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
