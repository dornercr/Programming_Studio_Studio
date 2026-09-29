#include "api.hpp"
struct Count::Impl{int x=7;};Count::Count():p(std::make_unique<Impl>()){}Count::~Count()=default;int Count::value()const{return p->x;}
