#include <climits>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <stdexcept>

static_assert(CHAR_BIT == 8, "8-bit bytes required");
struct Summary { std::size_t bytes; std::uint64_t sum; };
Summary summarize_bytes(const std::string& data) {
    if(data.size() > 1000000) throw std::length_error("input limit");
    std::uint64_t sum = 0;
    for(unsigned char byte : data) sum += byte;
    return {data.size(),sum};
}

static_assert('A' == 65 && 'c' == 99 && 'a' == 97 && 't' == 116);
int main() {
    const auto word = summarize_bytes("cat");
    assert(word.bytes == 3 && word.sum == 312);
    const std::string binary{char(0),char(255)};
    assert(summarize_bytes(binary).bytes == 2 && summarize_bytes(binary).sum == 255);
    assert(summarize_bytes("").bytes == 0);
    bool rejected = false;
    try { summarize_bytes(std::string(1000001,'x')); } catch(const std::length_error&) { rejected = true; }
    assert(rejected);
    std::cout << "bytes=" << word.bytes << " sum=" << word.sum << " binary-sum=" << summarize_bytes(binary).sum << '\n';
}
