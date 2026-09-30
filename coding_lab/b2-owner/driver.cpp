int main(){int n;if(!(std::cin>>n))return 2;auto p=makeReading(n);if(!p){std::cout<<"no reading\n";return 0;}std::cout<<*p<<"\n";}
