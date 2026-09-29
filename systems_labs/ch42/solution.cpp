#include <cassert>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

std::optional<std::pair<std::string,std::string>> parse(const std::string& line) {
    std::istringstream input(line);
    std::string left, pipe, right, extra;
    if(!(input >> left >> pipe >> right) || input >> extra) return std::nullopt;
    if(pipe != "|" || left == "|" || right == "|") return std::nullopt;
    return std::pair<std::string,std::string>{left,right};
}

int main() {
    const auto good = parse("emit | count");
    assert(good && good->first == "emit" && good->second == "count");
    assert(!parse("emit |") && !parse("| count"));
    assert(!parse("emit | count extra") && !parse("a | b | c"));
    std::cout << "left=" << good->first << " right=" << good->second << " invalid=rejected\n";
}
