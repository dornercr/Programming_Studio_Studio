#include "report.hpp"
#include "course_test.hpp"
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
