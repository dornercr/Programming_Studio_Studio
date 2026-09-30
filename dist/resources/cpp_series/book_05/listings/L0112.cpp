#include "check.hpp"
#include "metrics.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>
void self_test() {
    std::istringstream input{"1,5\n2,-2\n3,9\n"};
    const auto records=harbor::read_records(input);
    const auto baseline=harbor::analyze(records), candidate=harbor::analyze(records,true);
    CHECK(baseline.total==12 && baseline.count==3 && baseline.mean==4.0);
    CHECK(candidate.total==baseline.total && candidate.minimum==-2 && candidate.maximum==9);
    std::ostringstream output; harbor::write_report(output,baseline);
    CHECK(output.str()=="count=3 total=12 min=-2 max=9 mean=4.000\n");
    for (const char* text:{"", "2,4\n1,5\n", "1,2x\n", "1,1000001\n", "1,2,3\n"}) {
        std::istringstream bad{text}; bool rejected=false;
        try { (void)harbor::read_records(bad); } catch (const std::runtime_error&) { rejected=true; }
        CHECK(rejected);
    }
    std::cout << "PASS\n";
}
void benchmark() {
    std::vector<harbor::Record> records;
    for (std::uint64_t i=0;i<10003;++i) records.push_back({i,static_cast<int>(i%101)-50});
    for (bool lanes:{false,true}) {
        long long checksum=0;
        const auto start=std::chrono::steady_clock::now();
        for (int round=0;round<20;++round) {
            records[0].value=round;
            checksum+=harbor::analyze(records,lanes).total;
        }
        const auto elapsed=std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count();
        std::cout << "four_lanes=" << lanes << " elapsed_ms=" << elapsed << " checksum=" << checksum << '\n';
    }
    CHECK(harbor::analyze(records).total==harbor::analyze(records,true).total);
}
int main(int argc,char** argv) {
    try {
        if (argc==1 || (argc==2 && std::string_view(argv[1])=="--self-test")) { self_test(); return 0; }
        if (argc==2 && std::string_view(argv[1])=="--version") { std::cout << harbor::version << '\n'; return 0; }
        if (argc==2 && std::string_view(argv[1])=="--benchmark") { benchmark(); return 0; }
        if (argc==3 && std::string_view(argv[1])=="--input") {
            std::ifstream input(argv[2],std::ios::binary);
            if (!input) throw std::runtime_error("cannot open input");
            const auto records=harbor::read_records(input);
            harbor::write_report(std::cout,harbor::analyze(records));
            return 0;
        }
        std::cerr << "usage: harbor-metrics [--self-test|--version|--benchmark|--input FILE]\n";
        return 2;
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
