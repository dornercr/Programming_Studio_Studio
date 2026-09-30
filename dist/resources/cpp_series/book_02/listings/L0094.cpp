#include "course_test.hpp"
#include "../src/report.hpp"
#include <sstream>
#include <string>

int main() {
    harbor::Report report;
    std::istringstream good{"7|1|0|Keep this\n"};
    CHECK(!report.load(good));
    std::ostringstream before;
    report.write(before);
    std::istringstream invalid{"9|2|0|Candidate\n9|1|0|Duplicate\n"};
    const auto error = report.load(invalid);
    CHECK(error && error->line == 2);
    std::ostringstream after;
    report.write(after);
    CHECK(after.str() == before.str());
    std::istringstream replacement{"3|2|0|Replacement\n"};
    CHECK(!report.load(replacement));
    std::ostringstream changed;
    report.write(changed);
    CHECK(changed.str() == "open=1 total=1\n3|2|Replacement\n");
    std::istringstream empty;
    CHECK(!report.load(empty));
    std::ostringstream cleared;
    report.write(cleared);
    CHECK(cleared.str() == "open=0 total=0\n");
    course::report();
}
