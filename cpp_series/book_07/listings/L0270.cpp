#include <iostream>

int main() {
    struct Status{bool process=true,initialized=false,draining=false;
        bool startup()const{return initialized;}bool live()const{return process;}bool ready()const{return process&&initialized&&!draining;}}s;
    std::cout<<std::boolalpha<<"boot live="<<s.live()<<" ready="<<s.ready()<<'\n';
    s.initialized=true;std::cout<<"initialized startup="<<s.startup()<<" ready="<<s.ready()<<'\n';
    s.draining=true;std::cout<<"drain live="<<s.live()<<" ready="<<s.ready()<<'\n';
}
