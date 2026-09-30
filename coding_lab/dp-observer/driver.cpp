int main(){int n;if(!(std::cin>>n))return 2;Signal s;s.subscribe([](int x){std::cout<<"first="<<x<<"\n";});s.subscribe([](int x){std::cout<<"second="<<x<<"\n";});s.publish(n);}
