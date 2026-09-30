int main(){std::string s;std::getline(std::cin,s);auto render=bracket([](std::string text){return text;});std::cout<<render(s)<<"\n";}
