// Shared test support from Book II, B02-L0110.
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

#include <algorithm>
#include <numeric>
#include <optional>
#include <tuple>
#include <vector>
#include <utility>

struct Interval { int start, finish; };
std::vector<Interval> schedule(std::vector<Interval> jobs) {
    for (const auto job : jobs) if (job.start >= job.finish) throw std::invalid_argument("interval");
    std::sort(jobs.begin(), jobs.end(), [](const auto& a, const auto& b) {
        return std::tie(a.finish, a.start) < std::tie(b.finish, b.start);
    });
    std::vector<Interval> result;
    std::optional<int> last_finish;
    for (const auto job : jobs) {
        if (!last_finish || job.start >= *last_finish) {
            result.push_back(job); last_finish = job.finish;
        }
    }
    return result;
}
class DisjointSet {
    std::vector<std::size_t> parent_, rank_;
public:
    explicit DisjointSet(std::size_t n) : parent_(n), rank_(n) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }
    std::size_t find(std::size_t x) {
        if (parent_[x] != x) parent_[x] = find(parent_[x]);
        return parent_[x];
    }
    bool unite(std::size_t a, std::size_t b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) std::swap(a, b);
        parent_[b] = a;
        if (rank_[a] == rank_[b]) ++rank_[a];
        return true;
    }
};
struct Edge { std::size_t from, to; int weight; };
std::optional<long long> spanning_weight(std::size_t vertices, std::vector<Edge> edges) {
    if (vertices > 1000000) throw std::invalid_argument("teaching size limit");
    for (const auto edge : edges) {
        if (edge.from >= vertices || edge.to >= vertices) throw std::out_of_range("edge");
        if (edge.weight < -1000000000 || edge.weight > 1000000000) {
            throw std::invalid_argument("weight outside teaching range");
        }
    }
    if (vertices == 0) return 0;
    std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return std::tie(a.weight, a.from, a.to) < std::tie(b.weight, b.from, b.to);
    });
    DisjointSet components{vertices};
    long long sum{}; std::size_t accepted{};
    for (const auto edge : edges) {
        if (components.unite(edge.from, edge.to)) {
            sum += edge.weight; ++accepted;
            if (accepted == vertices - 1) break;
        }
    }
    if (accepted != vertices - 1) return std::nullopt;
    return sum;
}

int main() {
    const auto selected = schedule({{0, 6}, {1, 2}, {2, 4}, {4, 5}});
    CHECK(selected.size() == 3);
    CHECK(schedule({{-5, -3}, {-3, -1}}).size() == 2);
    CHECK(spanning_weight(4, {{0, 1, 1}, {1, 2, 2}, {2, 3, 3}, {0, 3, 20}}) == 6);
    CHECK(!spanning_weight(3, {{0, 1, 1}}));
    CHECK(spanning_weight(1, {}) == 0);
    CHECK(spanning_weight(0, {}) == 0);
    CHECK(spanning_weight(2, {{0, 1, -3}}) == -3);
    course::report();
}
