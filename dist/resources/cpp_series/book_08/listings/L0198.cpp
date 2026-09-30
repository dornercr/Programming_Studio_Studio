#include <future>
#include <string>
#include <iostream>

int main() {
    struct Context{std::string trace,span;};Context parent{"trace7","request"};
    auto job=std::async(std::launch::async,[parent]{return parent.trace+" parent="+parent.span+" child=worker";});
    std::cout<<job.get()<<'\n';
}
