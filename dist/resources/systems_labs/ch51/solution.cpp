#include <cassert>
#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <stdexcept>

struct Frame { std::string payload; std::size_t consumed; };
std::optional<Frame> frame(const std::vector<unsigned char>& bytes) {
    if(bytes.empty()) return std::nullopt;
    const std::size_t length = bytes[0];
    if(length > 8) throw std::invalid_argument("frame too large");
    if(bytes.size()-1 < length) return std::nullopt;
    return Frame{std::string(bytes.begin()+1,bytes.begin()+1+length),1+length};
}

int main() {
    std::vector<unsigned char> bytes{3,'c'};
    assert(!frame(bytes));
    bytes.insert(bytes.end(),{'a','t',1,'x'});
    const auto first = frame(bytes);
    assert(first && first->payload == "cat" && first->consumed == 4);
    bytes.erase(bytes.begin(),bytes.begin()+first->consumed);
    assert(frame(bytes)->payload == "x");
    bool rejected = false;
    try { frame({9}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "first=" << first->payload << " next=" << frame(bytes)->payload << '\n';
}
