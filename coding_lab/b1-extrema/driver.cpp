int main(){std::array<int,4> a{};for(int& x:a)if(!(std::cin>>x))return 2;auto [lo,hi]=extrema(a);std::cout<<lo<<" "<<hi<<"\n";}
