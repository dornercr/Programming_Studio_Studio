#include <iostream>
#include <string>
#include <functional>
#include <utility>

using Renderer=std::function<std::string(std::string)>;
Renderer bracket(Renderer next) {
    return [next=std::move(next)](std::string text){
        return "["+next(std::move(text))+"]";
    };
}
