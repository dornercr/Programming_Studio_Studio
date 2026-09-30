int main(){int n;if(!(std::cin>>n))return 2;try{auto reading=makeReading(n);std::cout<<*reading<<"\n";}catch(const std::invalid_argument&){std::cout<<"negative reading\n";}}
