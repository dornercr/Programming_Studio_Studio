#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <stdexcept>

std::string copy_chunks(const std::string& input, std::size_t read_limit, std::size_t write_limit) {
    if(read_limit == 0 || write_limit == 0) throw std::invalid_argument("no progress");
    std::string output;
    for(std::size_t pos=0; pos<input.size();) {
        const auto count = std::min(read_limit,input.size()-pos);
        const auto buffer = input.substr(pos,count);
        for(std::size_t sent=0; sent<buffer.size();) {
            const auto written = std::min(write_limit,buffer.size()-sent);
            output.append(buffer,sent,written);
            sent += written;
        }
        pos += count;
    }
    return output;
}

int main() {
    assert(copy_chunks("ABCDE",2,1) == "ABCDE");
    assert(copy_chunks("",2,1).empty());
    bool rejected = false;
    try { copy_chunks("A",2,0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << copy_chunks("ABCDE",2,1) << " short-writes=handled\n";
}
