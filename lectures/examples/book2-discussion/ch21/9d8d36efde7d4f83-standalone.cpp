// Exact original book listing B02-L0110
#ifndef CPP_COURSE_TEST_HPP
#define CPP_COURSE_TEST_HPP
#include <iostream>
#include <stdexcept>
#include <string>

// Test checks stay active even when NDEBUG is defined in optimized builds.
namespace course {
inline unsigned checks{};
inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(std::string(file) + ':' + std::to_string(line)
                                 + ": failed: " + expression);
    }
}
template<class Exception, class Function>
bool throws(Function&& function) {
    try { function(); }
    catch (const Exception&) { return true; }
    return false;
}
inline void report() { std::cout << "PASS checks=" << checks << '\n'; }
}
#define CHECK(...) ::course::check(static_cast<bool>((__VA_ARGS__)), \
                                  #__VA_ARGS__, __FILE__, __LINE__)
#endif

// Exact original book listing B02-L0090
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

// Exact original book listing B02-L0091
#include <algorithm>
#include <array>
#include <charconv>
#include <istream>
#include <ostream>
#include <ranges>
#include <set>
#include <string_view>
#include <tuple>
#include <variant>
#include <utility>
namespace harbor {
namespace {
std::optional<int> integer(std::string_view text) {
    if (text.empty()) return std::nullopt;
    int value{};
    const auto p = std::from_chars(text.data(), text.data() + text.size(), value);
    if (p.ec != std::errc{} || p.ptr != text.data() + text.size()) return std::nullopt;
    return value;
}
std::variant<Task, std::string> parse_line(const std::string& line) {
    std::array<std::string_view, 4> fields{};
    const std::string_view view{line};
    std::size_t begin{};
    for (std::size_t i = 0; i < 3; ++i) {
        const auto end = view.find('|', begin);
        if (end == std::string_view::npos) return std::string{"expected four fields"};
        fields[i] = view.substr(begin, end - begin);
        begin = end + 1;
    }
    fields[3] = view.substr(begin);
    auto id = integer(fields[0]);
    auto priority = integer(fields[1]);
    auto done = integer(fields[2]);
    if (!id || !priority || !done) return std::string{"invalid numeric field"};
    if (*id <= 0 || *priority < 1 || *priority > 3 || (*done != 0 && *done != 1)) {
        return std::string{"numeric field outside permitted range"};
    }
    if (fields[3].empty() || fields[3].size() > 200 ||
        fields[3].find('|') != std::string_view::npos) return std::string{"invalid title"};
    return Task{*id, *priority, *done == 1, std::string{fields[3]}};
}
}
std::optional<ParseError> Report::load(std::istream& input) {
    std::vector<Task> candidate;
    std::set<int> seen;
    std::string line;
    std::size_t number{};
    while (std::getline(input, line)) {
        ++number;
        if (line.size() > 4096 || candidate.size() >= 10000) {
            return ParseError{number, "input exceeds teaching limit"};
        }
        auto parsed = parse_line(line);
        if (const auto* error = std::get_if<std::string>(&parsed)) {
            return ParseError{number, *error};
        }
        Task task = std::get<Task>(std::move(parsed));
        if (!seen.insert(task.id).second) return ParseError{number, "duplicate id"};
        candidate.push_back(std::move(task));
    }
    if (input.bad() || (!input.eof() && input.fail())) {
        return ParseError{number + 1, "input read failed"};
    }
    tasks_.swap(candidate); // Commit only after every record has passed validation.
    return std::nullopt;
}
void Report::write(std::ostream& output) const {
    std::vector<Task> open;
    for (const Task& task : tasks_ | std::views::filter([](const Task& t) {
             return !t.done;
         })) {
        open.push_back(task);
    }
    std::ranges::sort(open, {}, [](const Task& task) {
        return std::tuple{task.priority, task.id};
    });
    output << "open=" << open.size() << " total=" << tasks_.size() << '\n';
    for (const Task& task : open) {
        output << task.id << '|' << task.priority << '|' << task.title << '\n';
    }
}
}

// Original book listing B02-L0094
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
