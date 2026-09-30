#include <iostream>
#include <string>
#include <functional>
#include <utility>

using Renderer=std::function<std::string(std::string)>;
Renderer bracket(Renderer next) {
    // TODO: return a callable that adds behavior around next.
    return next;
}
