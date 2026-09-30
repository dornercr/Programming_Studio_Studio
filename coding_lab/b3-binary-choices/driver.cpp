int main(){std::size_t n;if(!(std::cin>>n))return 2;try{for(auto& s:binaries(n))std::cout<<"["<<s<<"]\n";}catch(const std::invalid_argument&){std::cout<<"too long\n";}}
