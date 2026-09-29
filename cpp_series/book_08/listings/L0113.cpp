#include <csignal>
#include <chrono>
#include <thread>
#include <iostream>
volatile std::sig_atomic_t stop=0;
extern "C" void request_stop(int){stop=1;}
int main(){if(std::signal(SIGTERM,request_stop)==SIG_ERR)return 1;
 std::cout<<"ready=1\n"<<std::flush;
 while(!stop)std::this_thread::sleep_for(std::chrono::milliseconds(5));
 std::cout<<"ready=0\ndrained\n";
}
