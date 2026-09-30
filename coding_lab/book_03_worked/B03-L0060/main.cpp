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

#include <cstddef>
#include <functional>
#include <queue>
#include <vector>

using Graph = std::vector<std::vector<std::size_t>>;
void validate(const Graph& graph) {
    for (const auto& neighbors : graph) {
        for (auto v : neighbors) if (v >= graph.size()) throw std::out_of_range("vertex");
    }
}
std::vector<int> distances(const Graph& graph, std::size_t source) {
    validate(graph);
    if (source >= graph.size() || graph.size() > 1000000) throw std::out_of_range("source/size");
    std::vector<int> distance(graph.size(), -1);
    std::queue<std::size_t> pending;
    distance[source] = 0; pending.push(source);
    while (!pending.empty()) {
        const auto from = pending.front(); pending.pop();
        for (const auto to : graph[from]) {
            if (distance[to] != -1) continue;
            distance[to] = distance[from] + 1; pending.push(to);
        }
    }
    return distance;
}
bool directed_cycle(const Graph& graph) {
    validate(graph);
    std::vector<int> color(graph.size()); // 0 unseen, 1 active, 2 finished.
    std::function<bool(std::size_t)> visit = [&](std::size_t vertex) {
        color[vertex] = 1;
        for (auto next : graph[vertex]) {
            if (color[next] == 1) return true;
            if (color[next] == 0 && visit(next)) return true;
        }
        color[vertex] = 2; return false;
    };
    for (std::size_t v = 0; v < graph.size(); ++v) if (!color[v] && visit(v)) return true;
    return false;
}
std::size_t undirected_components(const Graph& graph) {
    validate(graph); // Caller additionally promises symmetric adjacency.
    std::vector<bool> seen(graph.size());
    std::size_t count{};
    for (std::size_t start = 0; start < graph.size(); ++start) {
        if (seen[start]) continue;
        ++count;
        std::vector<std::size_t> stack{start}; seen[start] = true;
        while (!stack.empty()) {
            auto from = stack.back(); stack.pop_back();
            for (auto to : graph[from]) if (!seen[to]) {
                seen[to] = true; stack.push_back(to);
            }
        }
    }
    return count;
}

int main() {
    const Graph undirected{{1, 2}, {0, 3}, {0}, {1}, {}};
    CHECK(distances(undirected, 0) == std::vector<int>({0, 1, 1, 2, -1}));
    CHECK(undirected_components(undirected) == 2);
    CHECK(!directed_cycle(Graph{{1, 2}, {2}, {}}));
    CHECK(directed_cycle(Graph{{1}, {2}, {0}}));
    CHECK(directed_cycle(Graph{{0}}));
    CHECK(!directed_cycle(Graph{}));
    CHECK(course::throws<std::out_of_range>([] { (void)distances(Graph{{9}}, 0); }));
    course::report();
}
