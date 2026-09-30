#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

struct Job {
    std::string format;
    int pages;
};
using Reply = std::optional<std::string>;
using Rule = std::function<Reply(const Job&)>;

Reply label_rule(const Job& job) {
    // No value means keep looking.
    if (job.format != "label") {
        return std::nullopt;
    }
    return "label desk";
}
Reply general_rule(const Job& job) {
    if (job.format == "label" || job.format == "report") {
        return "general desk";
    }
    return std::nullopt;
}

std::string route(const Job& job,
                  const std::vector<Rule>& rules) {
    if (job.pages < 1 || job.pages > 1000) {
        return "rejected";
    }
    // The first willing desk wins.
    for (const auto& rule : rules) {
        auto reply = rule(job);
        if (reply) {
            return *reply;
        }
    }
    return "unhandled";
}
void check(bool okay) {
    if (!okay) {
        throw std::runtime_error("routing check failed");
    }
}
Reply express_rule(const Job& job) {
    if (job.format == "report" && job.pages <= 5) {
        return "express desk";
    }
    return std::nullopt;
}
// Test helper: true only when the requested exception type is caught.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& f) {
    try { std::forward<F>(f)(); }
    catch (const E&) { return true; }
    return false;
}

int main() {
    const std::vector<Rule> rules{label_rule, general_rule};
    check(route({"label", 2}, rules) == "label desk");
    check(route({"report", 1000}, rules) == "general desk");
    check(route({"archive", 1}, rules) == "unhandled");
    check(route({"label", 0}, rules) == "rejected");
    check(route({"label", 1001}, rules) == "rejected");
    check(route({"label", 1}, {}) == "unhandled");
    const std::vector<Rule> reverse{general_rule, label_rule};
    check(route({"label", 1}, reverse) == "general desk");
    for (const Job& job : std::vector<Job>{
             {"label", 2}, {"report", 8}, {"archive", 1}, {"label", 0}}) {
        std::cout << route(job, rules) << '\n';
    }

    const std::vector<Rule> extended{express_rule, label_rule, general_rule};
    check(route({"report", 5}, extended) == "express desk");
    check(route({"report", 6}, extended) == "general desk");
    check(route({"label", 2}, extended) == "label desk");
    check(route({"report", 0}, extended) == "rejected");
    check(route({"archive", 1}, extended) == "unhandled");
    std::cout << "express: " << route({"report", 5}, extended) << '\n';
}
