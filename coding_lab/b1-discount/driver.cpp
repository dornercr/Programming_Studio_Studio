int main(){int amount,percent;if(!(std::cin>>amount>>percent))return 2;try{std::cout<<afterDiscount(amount,percent)<<"\n";}catch(const std::invalid_argument&){std::cout<<"range\n";}}
