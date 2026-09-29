#include <array>
#include <optional>
#include <string_view>
#include <iostream>

int main() {
    struct Node{std::string_view id;unsigned shard;bool ready;};
    const std::array nodes{Node{"a",0,false},Node{"b",1,true},Node{"c",0,true}};
    auto choose=[&](unsigned shard)->std::optional<std::string_view>{for(const auto&n:nodes)if(n.shard==shard&&n.ready)return n.id;return{};};
    auto retry=choose(0),missing=choose(2);
    std::cout<<"retry="<<*retry<<" absent-shard="<<std::boolalpha<<!missing<<'\n';
}
