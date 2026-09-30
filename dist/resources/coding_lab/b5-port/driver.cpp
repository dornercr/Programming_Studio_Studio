int main(){std::string s;std::getline(std::cin,s);try{std::cout<<parsePort(s)<<"\n";}catch(const std::invalid_argument&){std::cout<<"invalid port\n";}}
