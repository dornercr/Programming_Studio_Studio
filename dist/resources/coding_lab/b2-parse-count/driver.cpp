int main(){std::string word;std::getline(std::cin,word);auto n=parseCount(word);if(n)std::cout<<*n<<"\n";else std::cout<<"invalid\n";}
