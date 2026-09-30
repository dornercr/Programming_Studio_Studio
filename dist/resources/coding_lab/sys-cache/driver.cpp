int main(){std::uint64_t a,b,s;if(!(std::cin>>a>>b>>s))return 2;try{std::cout<<cacheSet(a,b,s)<<"\n";}catch(const std::invalid_argument&){std::cout<<"zero dimension\n";}}
