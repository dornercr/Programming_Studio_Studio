int main(){long long n;if(!(std::cin>>n))return 2;try{std::cout<<narrow(n)<<"\n";}catch(const std::out_of_range&){std::cout<<"outside int range\n";}}
