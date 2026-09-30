int main(){std::uint64_t n;if(!(std::cin>>n))return 2;try{std::cout<<pairCount(n)<<"\n";}catch(const std::invalid_argument&){std::cout<<"too many items\n";}}
