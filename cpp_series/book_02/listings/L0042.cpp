#include <iostream>
struct Engine{ bool running=false; void start(){running=true;} };
struct Dashboard{ void show(bool running) const{std::cout<<(running?"running":"stopped")<<'\n';} };
struct Car{ Engine engine; Dashboard dash; void start(){engine.start();dash.show(engine.running);} };
int main(){ Car c; c.start(); }
