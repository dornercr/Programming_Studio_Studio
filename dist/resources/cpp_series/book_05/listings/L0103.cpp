// widget.hpp
#pragma once
#include <memory>
class Widget{
public:
 Widget(); ~Widget();
 Widget(Widget&&) noexcept; Widget& operator=(Widget&&) noexcept;
 int compute() const;
private:
 class Impl;
 std::unique_ptr<Impl> p_;
};
