int main(){int n;if(!(std::cin>>n))return 2;std::cout<<applyAll(n,{[](int x){return x+2;},[](int x){return x*3;}})<<"\n";}
