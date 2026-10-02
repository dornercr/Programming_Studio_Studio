#ifndef HARBOR_REPORT_HPP
#define HARBOR_REPORT_HPP
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>
namespace harbor {
struct Task { int id; int priority; bool done; std::string title; };
struct ParseError { std::size_t line; std::string message; };
class Report {
    std::vector<Task> tasks_;
public:
    std::optional<ParseError> load(std::istream& input);
    void write(std::ostream& output) const;
};
}
#endif
