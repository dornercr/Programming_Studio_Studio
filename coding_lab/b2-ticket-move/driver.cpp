int main(){int id;if(!(std::cin>>id))return 2;Ticket first(id),second(std::move(first));std::cout<<first.id()<<" "<<second.id()<<"\n";}
