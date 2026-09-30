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

#include <optional>
#include <stdexcept>
#include <vector>

struct Edge { std::size_t to; int weight; };
class Graph {
    std::vector<std::vector<Edge>> adjacent_;
    bool directed_;
    static void prepare_append(std::vector<Edge>& edges) {
        if (edges.size() == edges.max_size()) throw std::length_error("edge capacity");
        if (edges.size() < edges.capacity()) return;
        const auto maximum = edges.max_size();
        const auto capacity = edges.capacity();
        const auto next = capacity == 0 ? std::size_t{1}
            : (capacity <= maximum / 2 ? capacity * 2 : maximum);
        edges.reserve(next);
    }
public:
    Graph(std::size_t vertices, bool directed) : adjacent_(vertices), directed_(directed) {}
    void add(std::size_t from, std::size_t to, int weight) {
        if (from >= adjacent_.size() || to >= adjacent_.size()) throw std::out_of_range("vertex");
        // Allocate both sides before adding either logical edge.
        prepare_append(adjacent_[from]);
        if (!directed_ && from != to) prepare_append(adjacent_[to]);
        adjacent_[from].push_back({to, weight});
        if (!directed_ && from != to) adjacent_[to].push_back({from, weight});
    }
    const std::vector<Edge>& neighbors(std::size_t vertex) const { return adjacent_.at(vertex); }
    std::vector<std::vector<std::optional<int>>> matrix() const {
        const auto size = adjacent_.size();
        std::vector<std::vector<std::optional<int>>> result(size,
            std::vector<std::optional<int>>(size));
        for (std::size_t from = 0; from < size; ++from) {
            for (const Edge edge : adjacent_[from]) {
                auto& cell = result[from][edge.to];
                if (!cell || edge.weight < *cell) cell = edge.weight;
            }
        }
        return result;
    }
};

int main() {
    Graph undirected{3, false};
    undirected.add(0, 1, 0); undirected.add(1, 2, 5);
    auto matrix = undirected.matrix();
    CHECK(matrix[0][1] == 0 && matrix[1][0] == 0);
    CHECK(!matrix[0][2]);
    undirected.add(1, 2, 3);
    CHECK(undirected.matrix()[1][2] == 3); // Parallel edges collapsed by minimum.
    Graph directed{2, true}; directed.add(0, 1, 7);
    CHECK(directed.neighbors(1).empty());
    CHECK(course::throws<std::out_of_range>([&] { directed.add(2, 0, 1); }));
    course::report();
}
