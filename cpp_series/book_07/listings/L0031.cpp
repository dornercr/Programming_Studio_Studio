#include "api.hpp"
#include <vector>
struct Count::Impl{std::vector<int> x{3,4};};Count::Count():p(std::make_unique<Impl>()){}Count::~Count()=default;int Count::value()const{return p->x[0]+p->x[1];}
