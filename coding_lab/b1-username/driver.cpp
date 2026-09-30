int main(){std::string line;std::getline(std::cin,line);try{std::cout<<userName(line)<<"\n";}catch(const std::invalid_argument&){std::cout<<"user record\n";}}
