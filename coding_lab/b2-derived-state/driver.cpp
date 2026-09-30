int main(){int id,value;if(!(std::cin>>id>>value))return 2;Meter m(id,value);std::cout<<m.id()<<" "<<m.reading()<<"\n";}
