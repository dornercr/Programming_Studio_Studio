int main(){std::size_t c,n,r;if(!(std::cin>>c>>n>>r))return 2;try{std::cout<<advance(c,n,r)<<"\n";}catch(const std::invalid_argument&){std::cout<<"invalid progress\n";}}
