#ifndef HARBOR_RELAY_HPP
#define HARBOR_RELAY_HPP
#include <cstddef>
#include <string>
#include <string_view>
namespace harbor {
struct RelayStats { std::size_t completed{}, failed{}; };
std::string process_request(std::string_view request);
RelayStats serve_n(int listener, std::size_t connections);
}
#endif
