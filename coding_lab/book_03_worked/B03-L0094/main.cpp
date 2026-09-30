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

#include <algorithm>
#include <limits>
#include <random>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
using harbor::Cost;
constexpr Cost inf = std::numeric_limits<Cost>::max();
std::vector<std::vector<Cost>> reference(const harbor::Graph& graph) {
    const auto n = graph.size();
    std::vector<std::vector<Cost>> d(n, std::vector<Cost>(n, inf));
    for (std::size_t i = 0; i < n; ++i) {
        d[i][i] = 0;
        for (const auto& edge : graph[i])
            d[i][edge.to] = std::min(d[i][edge.to], edge.weight);
    }
    for (std::size_t k = 0; k < n; ++k)
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                if (d[i][k] != inf && d[k][j] != inf)
                    d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    return d; // bounded tiny test graphs keep this arithmetic finite
}
void witness(const harbor::Request& request, const harbor::Route& route) {
    CHECK(!route.vertices.empty());
    CHECK(route.vertices.front() == request.source);
    CHECK(route.vertices.back() == request.target);
    CHECK(route.vertices.size() <= request.graph.size());
    Cost sum{};
    std::vector<bool> seen(request.graph.size());
    for (std::size_t i = 0; i < route.vertices.size(); ++i) {
        const auto vertex = route.vertices[i];
        CHECK(vertex < request.graph.size()); CHECK(!seen[vertex]);
        seen[vertex] = true;
        if (i == 0) continue;
        Cost edge_cost = inf;
        for (const auto& edge : request.graph[route.vertices[i - 1]])
            if (edge.to == vertex) edge_cost = std::min(edge_cost, edge.weight);
        CHECK(edge_cost != inf); sum += edge_cost;
    }
    CHECK(sum == route.distance);
}
class FailingInput : public std::streambuf {
    std::string text_;
    std::size_t position_{};
    std::size_t fail_at_;
    int_type underflow() override {
        if (position_ == fail_at_) throw std::ios_base::failure("injected");
        if (position_ == text_.size()) return traits_type::eof();
        return traits_type::to_int_type(text_[position_]);
    }
    int_type uflow() override {
        const auto value = underflow();
        if (!traits_type::eq_int_type(value, traits_type::eof())) ++position_;
        return value;
    }
public:
    FailingInput(std::string text, std::size_t position)
        : text_(std::move(text)), fail_at_(position) {}
};
class PrefixOutput : public std::streambuf {
public:
    std::string retained;
private:
    int_type overflow(int_type c) override {
        if (traits_type::eq_int_type(c, traits_type::eof()))
            return traits_type::not_eof(c);
        if (retained.size() == 5) return traits_type::eof();
        retained.push_back(traits_type::to_char_type(c)); return c;
    }
};
int main() {
    std::mt19937 random{901};
    for (int trial = 0; trial < 100; ++trial) {
        const std::size_t n = 1 + random() % 8;
        harbor::Request request{harbor::Graph(n), 0, 0};
        for (std::size_t from = 0; from < n; ++from)
            for (std::size_t to = 0; to < n; ++to)
                if (random() % 3 == 0)
                    request.graph[from].push_back({to, random() % 11});
        const auto expected = reference(request.graph);
        for (std::size_t from = 0; from < n; ++from)
            for (std::size_t to = 0; to < n; ++to) {
                request.source = from; request.target = to;
                const auto route = harbor::shortest_route(request);
                CHECK(route.has_value() == (expected[from][to] != inf));
                if (route) { CHECK(route->distance == expected[from][to]); witness(request, *route); }
            }
    }
    const std::string good = "4 4 0 3\n0 1 8\n0 2 1\n2 1 1\n1 3 1\n";
    std::istringstream initial(good);
    auto accepted = harbor::parse(initial);
    const auto original = accepted;
    CHECK(harbor::shortest_route(accepted)->distance == 3);
    for (std::size_t at = 0; at <= good.size(); ++at) {
        FailingInput buffer(good, at); std::istream input(&buffer);
        CHECK(course::throws<harbor::InputError>([&] { harbor::load(input, accepted); }));
        CHECK(accepted == original);
    }
    std::istringstream exceptional(good);
    exceptional.exceptions(std::ios::failbit | std::ios::badbit);
    CHECK(harbor::parse(exceptional) == original);
    for (const std::string bad : {"", "0 0 0 0", "1 0 0 0 x", "1 1 0 0 0 0 -1",
                                 "129 0 0 0", "1 4097 0 0", "1 0 1 0",
                                 "1 1 0 0 0 0 1000001", "1234567890123"}) {
        std::istringstream input(bad);
        CHECK(course::throws<harbor::InputError>([&] { harbor::load(input, accepted); }));
        CHECK(accepted == original);
    }
    std::vector<std::string> edges{"0 1 1", "0 2 1", "1 3 1", "2 3 1"};
    std::optional<harbor::Route> chosen;
    do {
        std::string text = "4 4 0 3\n";
        for (const auto& edge : edges) text += edge + '\n';
        std::istringstream input(text);
        const auto route = harbor::shortest_route(harbor::parse(input));
        if (!chosen) chosen = route; else CHECK(route == chosen);
    } while (std::next_permutation(edges.begin(), edges.end()));
    PrefixOutput buffer; std::ostream sink(&buffer);
    CHECK(course::throws<std::runtime_error>([&] { harbor::write(sink, chosen); }));
    CHECK(buffer.retained == "dista"); // External output is not rolled back.
    CHECK(course::throws<std::invalid_argument>([] {
        harbor::shortest_route({harbor::Graph(1), 1, 0});
    }));
    course::report();
}
