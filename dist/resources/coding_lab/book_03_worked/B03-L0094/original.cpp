#include "routes.hpp"
#include "course_test.hpp"
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
