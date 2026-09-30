int main(){int w,h;if(!(std::cin>>w>>h))return 2;Rectangle r(w,h);const Shape& shape=r;std::cout<<shape.area()<<"\n";}
