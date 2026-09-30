int main(){int count=0;{ScopeExit guard([&]{++count;});}std::cout<<count<<"\n";}
