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
