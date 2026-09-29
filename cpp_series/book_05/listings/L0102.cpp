// counter.hpp
#pragma once
#include <memory>
class Counter{
public:
 Counter(); ~Counter();
 Counter(Counter&&) noexcept; Counter& operator=(Counter&&) noexcept;
 void increment(); int value() const;
private:
 struct Impl; std::unique_ptr<Impl> impl_;
};
