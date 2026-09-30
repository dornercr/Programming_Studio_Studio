int main(){std::uint64_t n,a;if(!(std::cin>>n>>a))return 2;try{std::cout<<alignedSize(n,a)<<"\n";}catch(const std::exception& e){std::cout<<e.what()<<"\n";}}
