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

struct Change{std::size_t index;int delta;};
bool applyBatch(std::vector<int>& stock,const std::vector<Change>& changes){
 // BUG: writes directly to original state before knowing the whole batch works.
 for(const auto& c:changes){if(c.index>=stock.size())return false;int n=stock[c.index]+c.delta;if(n<0||n>1000)return false;stock[c.index]=n;}
 return true;
}

int main() {
    std::vector<int> v{5,2};
    bool ok=applyBatch(v,{{0,-1},{1,-3}});
    std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\n";
    }
