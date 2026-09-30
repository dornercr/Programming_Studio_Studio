#include <iostream>
#include <vector>
#include <optional>
#include <cstddef>

std::optional<std::size_t> firstMaximum(const std::vector<int>& v){
    if(v.empty()) return std::nullopt;
    std::size_t best=0;
    for(std::size_t i=1;i<v.size();++i)
        if(v[i]>v[best]) best=i; // Equal later items do not replace the first.
    return best;
}
