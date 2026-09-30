// Exact original book listing B02-L0104
#ifndef HARBOR_REPORTS_REPORT_HPP
#define HARBOR_REPORTS_REPORT_HPP
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace harbor {
struct Task {
    int id{};
    int priority{};
    bool done{};
    std::string title;
    bool operator==(const Task&) const = default;
};
enum class ErrorCode {
    input_io, line_limit, row_limit, field_count, integer_syntax,
    integer_range, id_domain, priority_domain, done_domain,
    title_domain, duplicate_id
};
struct LoadError {
    ErrorCode code;
    std::size_t line;
    bool operator==(const LoadError&) const = default;
};
std::string_view error_text(ErrorCode code) noexcept;
struct Limits {
    std::size_t max_rows{10'000};
    std::size_t max_line_bytes{256};
};

class Report {
public:
    // No returned error means the complete candidate replaced accepted state.
    // On a returned error or exception, accepted state is unchanged. Input
    // consumption is not rolled back. Invalid trusted limits throw.
    std::optional<LoadError> load(std::istream& input, Limits limits = {});
    std::vector<Task> snapshot() const { return accepted_; }
    std::string render(bool grouped = false) const;
    // Failure can leave a prefix in the external sink. It never changes tasks.
    void write(std::ostream& output, bool grouped = false) const;
private:
    std::vector<Task> accepted_;
};
} // namespace harbor
#endif

// Exact original book listing B02-L0105
#include <algorithm>
#include <array>
#include <charconv>
#include <istream>
#include <locale>
#include <limits>
#include <ostream>
#include <ranges>
#include <set>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <tuple>
#include <utility>
#include <variant>

namespace harbor {
namespace {
enum class ReadState { line, end, too_long, io_error };
ReadState read_line(std::istream& input, std::string& line,
                    std::size_t maximum) {
    line.clear();
    for (;;) {
        std::istream::int_type raw{};
        try {
            raw = input.get();
        } catch (const std::ios_base::failure&) {
            if (input.bad()) return ReadState::io_error;
            if (input.eof()) {
                return line.empty() ? ReadState::end : ReadState::line;
            }
            return ReadState::io_error;
        }
        if (std::istream::traits_type::eq_int_type(
                raw, std::istream::traits_type::eof())) {
            if (input.bad() || !input.eof()) return ReadState::io_error;
            return line.empty() ? ReadState::end : ReadState::line;
        }
        const char value = std::istream::traits_type::to_char_type(raw);
        if (value == '\n') return ReadState::line;
        // One excess byte is consumed, but it is never retained in line.
        if (line.size() == maximum) return ReadState::too_long;
        line.push_back(value);
    }
}
std::optional<ErrorCode> parse_integer(std::string_view text, int& value) {
    if (text.empty()) return ErrorCode::integer_syntax;
    const auto [end, error] = std::from_chars(
        text.data(), text.data() + text.size(), value);
    if (error == std::errc::result_out_of_range) {
        return ErrorCode::integer_range;
    }
    if (error != std::errc{} || end != text.data() + text.size()) {
        return ErrorCode::integer_syntax;
    }
    return std::nullopt;
}
std::variant<Task, ErrorCode> parse_task(std::string_view line) {
    if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
    std::array<std::string_view, 4> fields{};
    std::size_t start = 0;
    for (std::size_t index = 0; index < 3; ++index) {
        const auto separator = line.find('|', start);
        if (separator == std::string_view::npos) {
            return ErrorCode::field_count;
        }
        fields[index] = line.substr(start, separator - start);
        start = separator + 1;
    }
    fields[3] = line.substr(start);
    if (fields[3].find('|') != std::string_view::npos) {
        return ErrorCode::field_count;
    }
    int id{}, priority{}, done{};
    if (auto error = parse_integer(fields[0], id)) return *error;
    if (auto error = parse_integer(fields[1], priority)) return *error;
    if (auto error = parse_integer(fields[2], done)) return *error;
    if (id <= 0) return ErrorCode::id_domain;
    if (priority < 1 || priority > 3) return ErrorCode::priority_domain;
    if (done != 0 && done != 1) return ErrorCode::done_domain;
    const auto title = fields[3];
    const bool printable = std::ranges::all_of(title, [](char character) {
        const auto byte = static_cast<unsigned char>(character);
        return byte >= 0x20 && byte <= 0x7e;
    });
    const bool nonblank = std::ranges::any_of(title, [](char character) {
        return character != ' ';
    });
    if (title.empty() || title.size() > 200 || !printable || !nonblank) {
        return ErrorCode::title_domain;
    }
    return Task{id, priority, done == 1, std::string(title)};
}
} // namespace

std::string_view error_text(ErrorCode code) noexcept {
    switch (code) {
    case ErrorCode::input_io: return "input I/O failure";
    case ErrorCode::line_limit: return "line limit exceeded";
    case ErrorCode::row_limit: return "row limit exceeded";
    case ErrorCode::field_count: return "expected four fields";
    case ErrorCode::integer_syntax: return "invalid integer syntax";
    case ErrorCode::integer_range: return "integer out of range";
    case ErrorCode::id_domain: return "id must be positive";
    case ErrorCode::priority_domain: return "priority must be 1 through 3";
    case ErrorCode::done_domain: return "done must be 0 or 1";
    case ErrorCode::title_domain: return "invalid title";
    case ErrorCode::duplicate_id: return "duplicate id";
    }
    return "unknown input error";
}

std::optional<LoadError> Report::load(std::istream& input, Limits limits) {
    if (limits.max_rows == 0 || limits.max_rows > 1'000'000 ||
        limits.max_line_bytes == 0 || limits.max_line_bytes > 1'000'000) {
        throw std::invalid_argument("limits must be in [1, 1000000]");
    }
    std::vector<Task> candidate;
    candidate.reserve(std::min(limits.max_rows, std::size_t{256}));
    std::set<int> identifiers;
    std::string line;
    std::size_t number = 1;
    for (;;) {
        const auto state = read_line(input, line, limits.max_line_bytes);
        if (state == ReadState::end) break;
        if (state == ReadState::io_error) {
            return LoadError{ErrorCode::input_io, number};
        }
        if (state == ReadState::too_long) {
            return LoadError{ErrorCode::line_limit, number};
        }
        if (candidate.size() == limits.max_rows) {
            return LoadError{ErrorCode::row_limit, number};
        }
        auto parsed = parse_task(line);
        if (const auto* error = std::get_if<ErrorCode>(&parsed)) {
            return LoadError{*error, number};
        }
        auto task = std::get<Task>(std::move(parsed));
        if (!identifiers.insert(task.id).second) {
            return LoadError{ErrorCode::duplicate_id, number};
        }
        candidate.push_back(std::move(task));
        ++number;
    }
    accepted_.swap(candidate); // The only publication point for input state.
    return std::nullopt;
}

std::string Report::render(bool grouped) const {
    std::vector<Task> open;
    for (const auto& task : accepted_ | std::views::filter(
             [](const Task& value) { return !value.done; })) {
        open.push_back(task);
    }
    std::ranges::sort(open, {}, [](const Task& task) {
        return std::tuple{task.priority, task.id};
    });
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output << "open=" << open.size() << " total=" << accepted_.size() << '\n';
    if (grouped) {
        std::array<std::size_t, 3> counts{};
        for (const auto& task : open) {
            ++counts.at(static_cast<std::size_t>(task.priority - 1));
        }
        for (std::size_t index = 0; index < counts.size(); ++index) {
            output << "priority=" << index + 1
                   << " count=" << counts[index] << '\n';
        }
    }
    for (const auto& task : open) {
        output << task.id << '|' << task.priority << '|' << task.title << '\n';
    }
    return output.str();
}

void Report::write(std::ostream& output, bool grouped) const {
    const std::string completed = render(grouped);
    if (completed.size() > static_cast<std::size_t>(
            std::numeric_limits<std::streamsize>::max())) {
        throw std::length_error("report exceeds stream write range");
    }
    output.write(completed.data(), static_cast<std::streamsize>(completed.size()));
    if (!output) throw std::ios_base::failure("report output failed");
}
} // namespace harbor

// Original book listing B02-L0106
#include <fstream>
#include <iostream>
#include <new>
#include <string_view>

int main(int argc, char* argv[]) {
    try {
        bool grouped = false;
        const char* filename = nullptr;
        for (int index = 1; index < argc; ++index) {
            const std::string_view argument(argv[index]);
            if (argument == "--grouped") {
                grouped = true;
            } else if (argument == "--help") {
                std::cout << "usage: harbor_reports [--grouped] [FILE]\n";
                return std::cout ? 0 : 3;
            } else if (argument.starts_with('-') || filename != nullptr) {
                std::cerr << "usage: harbor_reports [--grouped] [FILE]\n";
                return 2;
            } else {
                filename = argv[index];
            }
        }
        std::ifstream file;
        std::istream* input = &std::cin; // Borrow; neither path owns cin.
        if (filename != nullptr) {
            file.open(filename, std::ios::binary);
            if (!file) {
                std::cerr << "cannot open input\n";
                return 2;
            }
            input = &file;
        }
        harbor::Report report;
        if (const auto error = report.load(*input)) {
            std::cerr << "line " << error->line << ": "
                      << harbor::error_text(error->code) << '\n';
            return 2;
        }
        report.write(std::cout, grouped);
        std::cout.flush();
        return std::cout ? 0 : 3;
    } catch (const std::bad_alloc&) {
        std::cerr << "resource allocation failed\n";
        return 3;
    } catch (const std::exception& error) {
        std::cerr << "operation failed: " << error.what() << '\n';
        return 3;
    }
}
