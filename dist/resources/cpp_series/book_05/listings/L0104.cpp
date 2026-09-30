#ifndef HARBOR_ACCUMULATOR_HPP
#define HARBOR_ACCUMULATOR_HPP
#include <cstddef>
#include <memory>
namespace harbor {
class Accumulator {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    Accumulator();
    ~Accumulator();
    Accumulator(Accumulator&&) noexcept;
    Accumulator& operator=(Accumulator&&) noexcept;
    Accumulator(const Accumulator&)=delete;
    Accumulator& operator=(const Accumulator&)=delete;
    void add(int value);
    long long total() const;
    std::size_t count() const;
};
}
#endif
