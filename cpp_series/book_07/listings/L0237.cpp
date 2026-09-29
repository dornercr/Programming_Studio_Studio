#include <array>
#include <string_view>
#include <iostream>

int main() {
    struct Span{std::string_view job,attempt,trace;int queued_ms,work_ms;};
    const std::array spans{Span{"job7","try1","traceA",20,5},Span{"job7","try2","traceA",10,6}};
    for(auto&s:spans)std::cout<<s.job<<' '<<s.attempt<<' '<<s.trace<<" wait="<<s.queued_ms<<" work="<<s.work_ms<<'\n';
}
