int main(){int age;if(!(std::cin>>age))return 2;try{std::cout<<nextAge(age)<<"\n";}catch(const std::invalid_argument&){std::cout<<"invalid age\n";}}
