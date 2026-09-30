#include <iostream>
#include <string_view>

struct Audit{virtual ~Audit()=default;virtual void record(std::string_view)=0;};class Controller{Audit& log_;bool running_=false;public:explicit Controller(Audit& a):log_(a){}bool start(){log_.record("started");if(running_)return false;running_=true;return true;}void stop(){running_=false;}bool running()const{return running_;}};
