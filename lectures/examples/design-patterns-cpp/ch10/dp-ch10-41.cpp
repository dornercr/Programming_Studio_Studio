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
int main() {
    Importer importer;
    const auto result = importer.run("4 6");
    check(result.count == 2 && result.total == 10);
    check(importer.run("-100 100").total == 0);
    check(importer.run("0").count == 1);
    for (const std::string bad : {"", "4x", "101", "1.5"}) {
        bool rejected = false;
        try { static_cast<void>(importer.run(bad)); }
        catch (const std::invalid_argument&) { rejected = true; }
        check(rejected);
    }
    std::string many;
    for (int i = 0; i < 1000; ++i) many += "100 ";
    check(importer.run(many).total == 100000);
    bool too_many = false;
    try { static_cast<void>(importer.run(many + "0")); }
    catch (const std::invalid_argument&) { too_many = true; }
    check(too_many);
    check(importer.mean("4 5") == 4.5);
    check(importer.mean("-100 100") == 0.0);
    bool empty_mean = false;
    try { static_cast<void>(importer.mean("")); }
    catch (const std::invalid_argument&) { empty_mean = true; }
    check(empty_mean);
    std::cout << "mean=" << std::fixed << std::setprecision(2)
              << importer.mean("4 5") << '\n';
    std::cout << "count=" << result.count << " total=" << result.total << '\n';
    std::cout << "syntax, range, and count errors rejected\n";
}
