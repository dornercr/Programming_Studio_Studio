int main(){std::size_t n,w;if(!(std::cin>>n>>w))return 2;try{for(auto [a,b]:partition(n,w))std::cout<<a<<" "<<b<<"\n";}catch(const std::invalid_argument&){std::cout<<"zero workers\n";}}
