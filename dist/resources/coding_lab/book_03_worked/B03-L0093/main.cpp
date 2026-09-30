// Book III source part B03-L0091
#ifndef HARBOR_ROUTES_HPP
#define HARBOR_ROUTES_HPP
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <optional>
#include <stdexcept>
#include <vector>
namespace harbor {
using Cost = std::uint64_t;
inline constexpr std::size_t max_vertices = 128;
inline constexpr std::size_t max_edges = 4096;
inline constexpr Cost max_weight = 1000000;
struct Edge {
    std::size_t to;
    Cost weight;
    bool operator==(const Edge&) const = default;
};
using Graph = std::vector<std::vector<Edge>>;
struct Request {
    Graph graph;
    std::size_t source{};
    std::size_t target{};
    bool operator==(const Request&) const = default;
};
struct Route {
    Cost distance;
    std::vector<std::size_t> vertices;
    bool operator==(const Route&) const = default;
};
class InputError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
Request parse(std::istream& input);
void load(std::istream& input, Request& accepted);
std::optional<Route> shortest_route(const Request& request);
void write(std::ostream& output, const std::optional<Route>& route);
}
#endif

// Book III source part B03-L0092
#include <algorithm>
#include <functional>
#include <istream>
#include <limits>
#include <ostream>
#include <queue>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
namespace harbor {
namespace {
bool space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\f' || c == '\v';
}
int next(std::istream& input) {
    try {
        const int c = input.get();
        if (c == std::char_traits<char>::eof() &&
            (input.bad() || !input.eof()))
            throw InputError("input failure");
        return c;
    } catch (const std::ios_base::failure&) {
        if (input.eof() && !input.bad())
            return std::char_traits<char>::eof();
        throw InputError("input failure");
    }
}
std::optional<std::string> token(std::istream& input) {
    std::string value;
    for (;;) {
        const int raw = next(input);
        if (raw == std::char_traits<char>::eof())
            return value.empty() ? std::nullopt : std::optional{value};
        const char c = static_cast<char>(raw);
        if (space(c)) {
            if (!value.empty()) return value;
            continue;
        }
        if (value.size() == 12) throw InputError("token too long");
        value.push_back(c);
    }
}
Cost number(std::istream& input, Cost limit) {
    const auto text = token(input);
    if (!text) throw InputError("missing field");
    Cost result{};
    for (char c : *text) {
        if (c < '0' || c > '9') throw InputError("unsigned integer required");
        const Cost digit = static_cast<Cost>(c - '0');
        if (digit > limit || result > (limit - digit) / 10)
            throw InputError("field exceeds limit");
        result = result * 10 + digit;
    }
    return result;
}
void validate(const Request& request) {
    const auto count = request.graph.size();
    if (count == 0 || count > max_vertices ||
        request.source >= count || request.target >= count)
        throw std::invalid_argument("invalid graph or query");
    std::size_t edges{};
    for (const auto& neighbors : request.graph) {
        if (neighbors.size() > max_edges - edges)
            throw std::invalid_argument("too many edges");
        edges += neighbors.size();
        for (const auto& edge : neighbors)
            if (edge.to >= count || edge.weight > max_weight)
                throw std::invalid_argument("invalid edge");
    }
}
}
Request parse(std::istream& input) {
    const auto vertices = static_cast<std::size_t>(number(input, max_vertices));
    if (vertices == 0) throw InputError("vertices must be positive");
    const auto edges = static_cast<std::size_t>(number(input, max_edges));
    const auto source = static_cast<std::size_t>(number(input, vertices - 1));
    const auto target = static_cast<std::size_t>(number(input, vertices - 1));
    Request candidate{Graph(vertices), source, target};
    for (std::size_t i = 0; i < edges; ++i) {
        const auto from = static_cast<std::size_t>(number(input, vertices - 1));
        const auto to = static_cast<std::size_t>(number(input, vertices - 1));
        const auto weight = number(input, max_weight);
        candidate.graph[from].push_back({to, weight});
    }
    if (token(input)) throw InputError("trailing field");
    for (auto& neighbors : candidate.graph)
        std::sort(neighbors.begin(), neighbors.end(), [](const Edge& a, const Edge& b) {
            return std::tie(a.to, a.weight) < std::tie(b.to, b.weight);
        });
    return candidate;
}
void load(std::istream& input, Request& accepted) {
    auto candidate = parse(input);
    static_assert(std::is_nothrow_move_assignable_v<Request>);
    accepted = std::move(candidate);
}
std::optional<Route> shortest_route(const Request& request) {
    validate(request);
    const auto count = request.graph.size();
    const auto infinity = std::numeric_limits<Cost>::max();
    std::vector<Cost> distance(count, infinity);
    std::vector<std::optional<std::size_t>> parent(count);
    using Entry = std::pair<Cost, std::size_t>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pending;
    distance[request.source] = 0;
    pending.push({0, request.source});
    while (!pending.empty()) {
        const auto [cost, vertex] = pending.top(); pending.pop();
        if (cost != distance[vertex]) continue; // stale immutable entry
        if (vertex == request.target) break;
        for (const auto& edge : request.graph[vertex]) {
            if (cost > infinity - edge.weight)
                throw std::overflow_error("distance overflow");
            const auto candidate = cost + edge.weight;
            if (candidate < distance[edge.to]) {
                distance[edge.to] = candidate;
                parent[edge.to] = vertex;
                pending.push({candidate, edge.to});
            }
        }
    }
    if (distance[request.target] == infinity) return std::nullopt;
    Route route{distance[request.target], {}};
    auto current = request.target;
    for (std::size_t steps = 0; steps < count; ++steps) {
        route.vertices.push_back(current);
        if (current == request.source) {
            std::reverse(route.vertices.begin(), route.vertices.end());
            return route;
        }
        if (!parent[current]) throw std::logic_error("missing predecessor");
        current = *parent[current];
    }
    throw std::logic_error("predecessor cycle");
}
void write(std::ostream& output, const std::optional<Route>& route) {
    if (!route) output << "unreachable\n";
    else {
        output << "distance=" << route->distance << "\npath=";
        for (std::size_t i = 0; i < route->vertices.size(); ++i) {
            if (i != 0) output << ',';
            output << route->vertices[i];
        }
        output << '\n';
    }
    if (!output) throw std::runtime_error("output failure");
}
}

#include <exception>
#include <iostream>
int main() {
    try {
        const auto request = harbor::parse(std::cin);
        harbor::write(std::cout, harbor::shortest_route(request));
        std::cout.flush();
        if (!std::cout) throw std::runtime_error("output failure");
        return 0;
    } catch (const harbor::InputError& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << '\n';
        return 3;
    }
}
