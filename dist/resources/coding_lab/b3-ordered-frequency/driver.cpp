int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(auto [key,n]:frequencies(v))std::cout<<key<<":"<<n<<"\n";}
