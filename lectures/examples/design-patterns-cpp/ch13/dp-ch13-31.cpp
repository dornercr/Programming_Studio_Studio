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
// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  return route({
    "label",2
  },{
    express_rule,label_rule
  });
})() << '\n';
}
