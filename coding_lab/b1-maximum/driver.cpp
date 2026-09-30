int main(){ std::vector<int> v; int x; while(std::cin>>x)v.push_back(x); try{std::cout<<maximum(v)<<"\n";}catch(const std::invalid_argument&){std::cout<<"empty input\n";} }
