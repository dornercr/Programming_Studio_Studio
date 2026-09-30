#include <array>
#include <optional>
#include <string_view>
#include <iostream>

int main() {
    struct Node{std::string_view id;bool alive,ready;unsigned in_flight;};
    const std::array nodes{Node{"a",true,false,0},Node{"b",true,true,5},Node{"c",true,true,2}};
    std::optional<std::size_t> choice;
    for(std::size_t i=0;i<nodes.size();++i)if(nodes[i].alive&&nodes[i].ready)
        if(!choice||nodes[i].in_flight<nodes[*choice].in_flight)choice=i;
    if(!choice)return 1;std::cout<<"route="<<nodes[*choice].id<<" in-flight="<<nodes[*choice].in_flight<<'\n';
}
