#ifndef HARBOR_SUMMARY_HPP
#define HARBOR_SUMMARY_HPP
#include <cstddef>
#include <span>
namespace harbor {
struct Summary { std::size_t count; long long total; double mean; };
Summary summarize(std::span<const int> values);
}
#endif
