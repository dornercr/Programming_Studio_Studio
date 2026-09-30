#include <cassert>
#include <iostream>
#include <string>
#include <utility>

std::pair<std::string,unsigned> buffered(const std::string& input) {
    std::string pending, output; unsigned flushes = 0;
    const auto flush = [&] {
        if(pending.empty()) return;
        output += pending; pending.clear(); ++flushes;
    };
    for(char ch : input) { pending += ch; if(pending.size() == 4) flush(); }
    flush();
    return {output,flushes};
}

int main() {
    const auto result = buffered("ABCDE");
    assert(result.first == "ABCDE" && result.second == 2);
    assert(buffered("ABCD").second == 1 && buffered("").second == 0);
    std::cout << result.first << " batches=" << result.second << '\n';
}
