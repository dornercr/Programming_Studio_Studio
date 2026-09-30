int main(){Snapshot first({1,2});Snapshot second=first;second.set(0,9);std::cout<<first.get(0)<<" "<<second.get(0)<<"\n";}
