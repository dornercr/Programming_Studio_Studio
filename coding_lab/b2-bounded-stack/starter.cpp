#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include <utility>
#include <cstddef>

template<class T>class Stack{std::vector<T> values_;std::size_t cap_;public:explicit Stack(std::size_t c):cap_(c){}bool push(T value){values_.push_back(std::move(value));return true;}std::optional<T> pop(){if(values_.empty())return std::nullopt;T item=values_.back();values_.pop_back();return item;}std::size_t size()const{return values_.size();}};
