int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);auto p=firstMaximum(v);if(p)std::cout<<*p<<"\n";else std::cout<<"empty\n";}
