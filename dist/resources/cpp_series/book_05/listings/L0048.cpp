#include <string>
#include <cassert>

struct Config{std::string prefix;};
class Formatter{
 const Config& config_;
public:
 explicit Formatter(const Config& c):config_(c){}
 std::string format(std::string s)const{return config_.prefix+s;}
};
int main(){Config c{"test:"};Formatter f(c);assert(f.format("ok")=="test:ok");}
