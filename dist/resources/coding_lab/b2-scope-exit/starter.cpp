#include <iostream>
#include <functional>
#include <utility>

class ScopeExit{std::function<void()> fn_;bool active_=true;public:explicit ScopeExit(std::function<void()> f):fn_(std::move(f)){}~ScopeExit(){}ScopeExit(const ScopeExit&)=delete;ScopeExit& operator=(const ScopeExit&)=delete;void dismiss(){active_=false;}};
