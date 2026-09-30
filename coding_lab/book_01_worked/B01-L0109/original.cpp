#include <iostream>
#include <string>
class User{
    std::string name_;
    int level_{1};
public:
    explicit User(std::string n):name_(std::move(n)){}
    User(std::string n,int level):User(std::move(n)){ level_=level; }
    void print() const { std::cout << name_ << ' ' << level_ << '\n'; }
};
int main(){ User a{"Ada"}; User b{"Linus",3}; a.print(); b.print(); }
