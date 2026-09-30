int main(){int has,value,fallback;if(!(std::cin>>has>>value>>fallback))return 2;std::cout<<selectedValue(has?&value:nullptr,fallback)<<"\n";}
