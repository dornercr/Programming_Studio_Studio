#include <iostream>
enum class Status{ queued, running, done };
const char* name(Status s){
    switch(s){case Status::queued:return "queued";case Status::running:return "running";case Status::done:return "done";}
    return "unknown";
}
int main(){ std::cout << name(Status::running) << '\n'; }
