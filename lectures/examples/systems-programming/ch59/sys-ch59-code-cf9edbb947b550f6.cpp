#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <charconv>
#include <system_error>

std::optional<int> parse_version(const std::string& candidate) {
    const std::string prefix = "version=";
    if(candidate.compare(0,prefix.size(),prefix) != 0) return std::nullopt;
    int value = 0;
    const char* first = candidate.data()+prefix.size();
    const char* last = candidate.data()+candidate.size();
    const auto parsed = std::from_chars(first,last,value);
    if(parsed.ec != std::errc{} || parsed.ptr != last || value < 1 || value > 99) return std::nullopt;
    return value;
}
bool update(std::string& live, const std::string& candidate) {
    const auto value = parse_version(candidate);
    if(!value) return false;
    std::string prepared = "version="+std::to_string(*value);
    live.swap(prepared);
    return true;
}

int main() {
    std::string live = "version=1";
    for(const std::string candidate : {"version=","version=2x","version=0","version=100"}) {
        assert(!update(live,candidate) && live == "version=1");
    }
    assert(update(live,"version=2") && live == "version=2");
    std::cout << live << " rejected-updates=unchanged\n";
}
