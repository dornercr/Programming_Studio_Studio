#include <cassert>
#include <cstddef>
#include <iostream>
#include <limits>
#include <optional>
#include <string>

std::optional<std::string> read_slice(const std::string& file, std::size_t offset,
                                     std::size_t length) {
    if(offset > file.size() || length > file.size()-offset) return std::nullopt;
    return file.substr(offset,length);
}

int main() {
    assert(read_slice("ABCDE",2,3) == "CDE");
    assert(read_slice("ABCDE",5,0) == "");
    assert(!read_slice("ABCDE",6,0));
    assert(!read_slice("ABCDE",2,std::numeric_limits<std::size_t>::max()));
    std::cout << *read_slice("ABCDE",2,3) << " end-empty=valid\n";
}
