#include <optional>
#include <iostream>

int main() {
    struct Reserve{int quantity;};struct Reserved{int quantity;int remaining;};int stock=5;
    auto handle=[&](Reserve c)->std::optional<Reserved>{if(c.quantity<1||c.quantity>stock)return{};stock-=c.quantity;return Reserved{c.quantity,stock};};
    auto event=handle({3}),rejected=handle({4});
    std::cout<<"reserved="<<event->quantity<<" remaining="<<event->remaining<<" second-event="<<std::boolalpha<<bool(rejected)<<'\n';
}
