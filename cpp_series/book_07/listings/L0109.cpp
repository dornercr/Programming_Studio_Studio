#include <array>
#include <set>
#include <iostream>

int main() {
    enum class Stage{received,persisted,applied};
    struct Ack{unsigned node,index;Stage stage;};
    std::array acks{Ack{1,9,Stage::persisted},Ack{2,9,Stage::received},Ack{1,9,Stage::applied},Ack{3,9,Stage::persisted}};
    std::set<unsigned> durable;
    for(auto a:acks)if(a.index==9&&a.stage!=Stage::received)durable.insert(a.node);
    std::cout<<"distinct-durable="<<durable.size()<<" majority-of3="<<std::boolalpha<<(durable.size()>=2)<<'\n';
}
