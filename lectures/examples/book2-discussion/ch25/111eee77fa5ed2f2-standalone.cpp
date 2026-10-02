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

// Original book listing B02-L0109
#include <algorithm>
#include <array>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

// These stream buffers fail deliberately; no disk or network is required.
class BrokenInput final : public std::streambuf {
public:
    BrokenInput(std::string text, std::size_t failure)
        : text_(std::move(text)), failure_(failure) {}
    std::size_t consumed() const noexcept { return position_; }
protected:
    int_type underflow() override {
        if (position_ == failure_) {
            throw std::ios_base::failure("injected read failure");
        }
        return position_ == text_.size() ? traits_type::eof()
            : traits_type::to_int_type(text_[position_]);
    }
    int_type uflow() override {
        const auto value = underflow();
        if (!traits_type::eq_int_type(value, traits_type::eof())) ++position_;
        return value;
    }
private:
    std::string text_;
    std::size_t failure_;
    std::size_t position_{};
};
class LimitedOutput final : public std::streambuf {
public:
    explicit LimitedOutput(std::size_t capacity) : capacity_(capacity) {}
    const std::string& bytes() const noexcept { return bytes_; }
protected:
    std::streamsize xsputn(const char* data, std::streamsize count) override {
        std::streamsize written = 0;
        while (written < count && bytes_.size() < capacity_) {
            bytes_.push_back(data[written++]);
        }
        return written;
    }
    int_type overflow(int_type character) override {
        if (traits_type::eq_int_type(character, traits_type::eof())) {
            return traits_type::not_eof(character);
        }
        if (bytes_.size() == capacity_) return traits_type::eof();
        bytes_.push_back(traits_type::to_char_type(character));
        return character;
    }
private:
    std::size_t capacity_;
    std::string bytes_;
};

int main() {
    using harbor::ErrorCode;
    harbor::Report report;
    std::istringstream initial("9|2|0|Existing\n");
    CHECK(!report.load(initial));
    const auto accepted = report.snapshot();
    const auto rendered = report.render();
    const std::vector<std::pair<std::string, ErrorCode>> invalid{
        {"", ErrorCode::field_count},
        {"1|2|0", ErrorCode::field_count},
        {"1|2|0|a|b", ErrorCode::field_count},
        {"+1|2|0|a", ErrorCode::integer_syntax},
        {"1x|2|0|a", ErrorCode::integer_syntax},
        {"0|2|0|a", ErrorCode::id_domain},
        {"-1|2|0|a", ErrorCode::id_domain},
        {"1|0|0|a", ErrorCode::priority_domain},
        {"1|4|0|a", ErrorCode::priority_domain},
        {"1|2|2|a", ErrorCode::done_domain},
        {"1|2|0|", ErrorCode::title_domain},
        {"1|2|0|   ", ErrorCode::title_domain},
        {"1|2|0|a\tb", ErrorCode::title_domain},
        {"1|2|0|" + std::string(201, 'a'), ErrorCode::title_domain},
        {std::string(100, '9') + "|2|0|a", ErrorCode::integer_range},
        {"1|2|0|a\n1|1|0|b", ErrorCode::duplicate_id}
    };
    for (const auto& [text, code] : invalid) {
        std::istringstream input(text + '\n');
        const auto error = report.load(input);
        CHECK(error && error->code == code);
        CHECK(report.snapshot() == accepted);
        CHECK(report.render() == rendered);
    }
    // Rows and retained line bytes have independently tested limits.
    {
        std::istringstream input("1|1|0|a\n2|1|0|b\n");
        CHECK(report.load(input, {1, 256}) ==
              harbor::LoadError{ErrorCode::row_limit, 2});
        CHECK(report.snapshot() == accepted);
    }
    {
        std::istringstream input("1|1|0|ab\n");
        CHECK(report.load(input, {1, 7}) ==
              harbor::LoadError{ErrorCode::line_limit, 1});
        CHECK(report.snapshot() == accepted);
        std::istringstream exact("1|1|0|a\n");
        CHECK(!report.load(exact, {1, 7}));
        CHECK(report.snapshot().at(0).title == "a");
    }
    {
        std::istringstream input("1|1|0|a\n");
        CHECK(course::throws<std::invalid_argument>([&] {
            report.load(input, {0, 256});
        }));
    }
    // Accepted strings do not borrow the parser's input storage.
    {
        std::string source("4|3|0|Owned title");
        std::istringstream input(source);
        CHECK(!report.load(input));
        source.assign(100, 'x');
        CHECK(report.snapshot().at(0).title == "Owned title");
    }
    const auto before_io = report.snapshot();
    const std::string candidate("1|1|0|New\n2|2|0|Next\n");
    for (std::size_t failure = 0; failure < candidate.size(); ++failure) {
        for (const bool throw_on_bad : {false, true}) {
            BrokenInput buffer(candidate, failure);
            std::istream input(&buffer);
            if (throw_on_bad) input.exceptions(std::ios::badbit);
            const auto error = report.load(input);
            CHECK(error && error->code == ErrorCode::input_io);
            CHECK(report.snapshot() == before_io);
            CHECK(buffer.consumed() == failure);
        }
    }
    // Clean EOF remains success even with failbit/badbit exceptions enabled.
    for (const std::string& text : {std::string{}, std::string{"1|1|0|a"},
                                  std::string{"1|1|0|a\n"}}) {
        std::istringstream input(text);
        input.exceptions(std::ios::failbit | std::ios::badbit);
        CHECK(!report.load(input));
    }
    // Arrival order is not report order: verify every permutation of 4 rows.
    std::array<std::string, 4> rows{
        "1|2|0|a\n", "2|1|0|b\n", "3|1|0|c\n", "4|3|1|d\n"};
    const std::string expected("open=3 total=4\n2|1|b\n3|1|c\n1|2|a\n");
    do {
        std::string text;
        for (const auto& row : rows) text += row;
        std::istringstream input(text);
        CHECK(!report.load(input));
        CHECK(report.render() == expected);
    } while (std::next_permutation(rows.begin(), rows.end()));
    CHECK(report.render(true) ==
          "open=3 total=4\npriority=1 count=2\npriority=2 count=1\n"
          "priority=3 count=0\n2|1|b\n3|1|c\n1|2|a\n");
    const auto before_write = report.snapshot();
    for (const bool throw_on_bad : {false, true}) {
        LimitedOutput buffer(8);
        std::ostream output(&buffer);
        if (throw_on_bad) output.exceptions(std::ios::badbit);
        CHECK(course::throws<std::ios_base::failure>([&] {
            report.write(output);
        }));
        CHECK(buffer.bytes() == expected.substr(0, 8));
        CHECK(report.snapshot() == before_write);
    }
    // A valid empty replacement is a successful state transition, not failure.
    std::istringstream empty;
    CHECK(!report.load(empty));
    CHECK(report.snapshot().empty());
    CHECK(report.render() == "open=0 total=0\n");
    course::report();
}
