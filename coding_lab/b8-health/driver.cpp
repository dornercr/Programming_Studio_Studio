int main(){Health h=Health::starting;std::string event;while(std::cin>>event)h=transition(h,event);std::cout<<static_cast<int>(h)<<"\n";}
