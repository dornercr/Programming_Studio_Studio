int main(){IdempotentCounter c;std::string key;int delta;while(std::cin>>key>>delta)c.apply(key,delta);std::cout<<c.total()<<"\n";}
