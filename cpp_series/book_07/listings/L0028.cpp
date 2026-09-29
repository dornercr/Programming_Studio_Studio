#pragma once
#include <memory>
class Count{public:Count();~Count();int value()const;private:struct Impl;std::unique_ptr<Impl> p;};
