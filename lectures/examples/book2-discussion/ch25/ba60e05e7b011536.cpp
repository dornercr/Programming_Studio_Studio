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
