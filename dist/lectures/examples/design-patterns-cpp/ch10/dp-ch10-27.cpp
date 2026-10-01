#include <charconv>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void check(bool ok) {
    if (!ok) throw std::runtime_error("check failed");
}
class Parser {
public:
    std::vector<int> parse(const std::string& text) const {
        std::istringstream input(text);
        std::vector<int> values;
        std::string token;
        while (input >> token) {
            int n = 0;
            const char* end = token.data() + token.size();
            const auto result = std::from_chars(token.data(), end, n);
            if (result.ec != std::errc{} || result.ptr != end) {
                throw std::invalid_argument("integer");
            }
            values.push_back(n);
        }
        return values;
    }
};
class Validator {
public:
    void validate(const std::vector<int>& v) const {
        if (v.empty() || v.size() > 1000) {
            throw std::invalid_argument("count");
        }
        for (int n : v) {
            if (n < -100 || n > 100) {
                throw std::invalid_argument("range");
            }
        }
    }
};
struct Stats { std::size_t count; int total; };
class Summarizer {
public:
    // Precondition: Validator accepted v.
    Stats summarize(
        const std::vector<int>& v) const {
        int total = 0;
        for (int n : v) total += n;
        return {v.size(), total};
    } // End summary.
};
class Importer {
    Parser parser_;
    Validator validator_;
    Summarizer summarizer_;
public:
    double mean(const std::string& text) const {
        const auto result = run(text);
        return static_cast<double>(result.total) / result.count;
    }
    Stats run(
        const std::string& text) const {
        // Validate before calculating.
        const auto v = parser_.parse(text);
        validator_.validate(v);
        return summarizer_.summarize(v);
    } // End import.
};
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  return dpx_rejects<std::invalid_argument>([]{
    Importer{
    }.run("7x");
  });
})() << '\n';
}
