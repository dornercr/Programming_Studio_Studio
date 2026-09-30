#include <array>
#include <iostream>

int main() {
    struct Pool{unsigned used=0,limit;bool acquire(){if(used==limit)return false;++used;return true;}void release(){if(used)--used;}};
    Pool report{0,1},interactive{0,2};
    bool first=report.acquire(),overflow=report.acquire(),user=interactive.acquire();
    std::cout<<std::boolalpha<<"report="<<first<<" report-overflow="<<overflow<<" interactive="<<user<<'\n';
    report.release();interactive.release();
}
