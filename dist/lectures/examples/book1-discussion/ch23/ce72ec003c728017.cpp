#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>

struct LoggedMember {
    std::string name;
    std::vector<std::string>& log;
    LoggedMember(std::string n, std::vector<std::string>& out) : name(std::move(n)), log(out) { log.push_back(name + "+"); }
    ~LoggedMember() { log.push_back(name + "-"); }
};
struct LoggedOwner {
    std::vector<std::string>& log;
    LoggedMember a, b;
    LoggedOwner(std::vector<std::string>& out, bool fail) : log(out), a("A", out), b("B", out) {
        log.push_back("body");
        if (fail) throw std::runtime_error("construction");
    }
    ~LoggedOwner() { log.push_back("owner-"); }
};
std::string cleanupTrace(bool fail) {
    // Reserve enough log entries before any destructor records its message.
    std::vector<std::string> log;
    log.reserve(8);
    try { LoggedOwner owner(log, fail); }
    catch (const std::runtime_error&) { log.push_back("caught"); }
    std::string joined;
    for (const auto& event : log) { if (!joined.empty()) joined += ' '; joined += event; }
    return joined;
}

int main() {
    std::cout<<cleanupTrace(true)<<"\n";
    }
