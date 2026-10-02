#include <iostream>
#include <string_view>
struct Logger{ virtual ~Logger()=default; virtual void write(std::string_view)=0; };
struct ConsoleLogger:Logger{ void write(std::string_view s) override{std::cout<<s<<'\n';} };
struct Service{ Logger& log; void run(){log.write("service running");} };
int main(){ ConsoleLogger l; Service s{l}; s.run(); }
