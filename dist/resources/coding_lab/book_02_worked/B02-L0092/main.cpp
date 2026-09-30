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

// Original book listing B02-L0092
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc > 2) { std::cerr << "usage: report [input-file]\n"; return 1; }
    std::ifstream file;
    std::istream* input = &std::cin; // Borrow, never delete.
    if (argc == 2) {
        file.open(argv[1]);
        if (!file) { std::cerr << "error: cannot open input\n"; return 1; }
        input = &file;
    }
    harbor::Report report;
    if (const auto error = report.load(*input)) {
        std::cerr << "error line " << error->line << ": " << error->message << '\n';
        return 1;
    }
    report.write(std::cout);
    if (!std::cout) { std::cerr << "error: output failed\n"; return 1; }
}
