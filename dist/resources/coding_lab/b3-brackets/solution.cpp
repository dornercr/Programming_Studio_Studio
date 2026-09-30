#include <iostream>
#include <string>
#include <string_view>
#include <vector>

bool balanced(std::string_view text) {
    std::vector<char> stack;
    for(char ch:text){
        if(ch=='('||ch=='['||ch=='{') stack.push_back(ch);
        else if(ch==')'||ch==']'||ch=='}'){
            if(stack.empty()) return false;
            char wanted=ch==')'?'(':ch==']'?'[':'{';
            if(stack.back()!=wanted) return false;
            stack.pop_back();
        }
    }
    return stack.empty();
}
