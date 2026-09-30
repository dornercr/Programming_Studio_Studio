#include "accumulator.hpp"
#include <stdexcept>
namespace harbor {
struct Accumulator::Impl { long long total{}; std::size_t count{}; };
Accumulator::Accumulator():impl_(std::make_unique<Impl>()) {}
Accumulator::~Accumulator()=default;
Accumulator::Accumulator(Accumulator&&) noexcept=default;
Accumulator& Accumulator::operator=(Accumulator&&) noexcept=default;
void Accumulator::add(int value) {
    if (!impl_) throw std::logic_error("moved-from accumulator");
    if (value < -1000000 || value > 1000000 || impl_->count==1000000)
        throw std::invalid_argument("accumulator bounds");
    impl_->total+=value; ++impl_->count;
}
long long Accumulator::total() const {
    if (!impl_) throw std::logic_error("moved-from accumulator");
    return impl_->total;
}
std::size_t Accumulator::count() const {
    if (!impl_) throw std::logic_error("moved-from accumulator");
    return impl_->count;
}
}
