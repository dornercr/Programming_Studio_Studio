#include "course_test.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <queue>
#include <stdexcept>
#include <vector>
using Graph = std::vector<std::vector<std::size_t>>;
void validate(const Graph& graph) {
    for (const auto& neighbors : graph)
        for (auto vertex : neighbors)
            if (vertex >= graph.size()) throw std::out_of_range("edge");
}
std::optional<std::vector<std::size_t>> path(
    const Graph& graph, std::size_t source, std::size_t target) {
    validate(graph);
    if (source >= graph.size() || target >= graph.size())
        throw std::out_of_range("vertex");
    std::vector<std::optional<std::size_t>> parent(graph.size());
    std::queue<std::size_t> pending;
    parent[source] = source;
    pending.push(source);
    while (!pending.empty()) {
        const auto current = pending.front(); pending.pop();
        for (auto next : graph[current]) if (!parent[next]) {
            parent[next] = current; pending.push(next);
        }
    }
    if (!parent[target]) return std::nullopt;
    std::vector<std::size_t> result;
    for (auto current = target; ; current = *parent[current]) {
        result.push_back(current);
        if (current == source) break;
    }
    std::reverse(result.begin(), result.end());
    return result;
}
bool cycle(const Graph& graph) {
    validate(graph);
    struct Frame { std::size_t vertex; std::size_t next; };
    std::vector<unsigned char> color(graph.size());
    std::vector<Frame> stack;
    for (std::size_t root = 0; root < graph.size(); ++root) {
        if (color[root] != 0) continue;
        color[root] = 1; stack.push_back({root, 0});
        while (!stack.empty()) {
            auto& frame = stack.back();
            if (frame.next == graph[frame.vertex].size()) {
                color[frame.vertex] = 2; stack.pop_back(); continue;
            }
            const auto next = graph[frame.vertex][frame.next++];
            if (color[next] == 1) return true;
            if (color[next] == 0) {
                color[next] = 1; stack.push_back({next, 0});
                // frame may now be invalid; it is never used after this push.
            }
        }
    }
    return false;
}
int main() {
    const Graph dag{{1, 2}, {3}, {3}, {}, {}};
    CHECK(path(dag, 0, 3) == std::vector<std::size_t>({0, 1, 3}));
    CHECK(!path(dag, 0, 4));
    CHECK(path(dag, 2, 2) == std::vector<std::size_t>({2}));
    CHECK(!cycle(dag)); CHECK(cycle(Graph{{1}, {2}, {0}}));
    CHECK(cycle(Graph{{0}})); CHECK(!cycle(Graph{}));
    CHECK(course::throws<std::out_of_range>([] { cycle(Graph{{1}}); }));
    Graph chain(20000);
    for (std::size_t i = 1; i < chain.size(); ++i) chain[i - 1].push_back(i);
    CHECK(!cycle(chain));
    CHECK(path(chain, 0, chain.size() - 1)->size() == chain.size());
    chain.back().push_back(0); CHECK(cycle(chain));
    course::report();
}
