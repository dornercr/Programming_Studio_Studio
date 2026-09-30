#include <string_view>
#include <iostream>
int main(int argc,char**argv){if(argc!=2||std::string_view(argv[1])!="check"){std::cerr<<"invalid mode\n";return 2;}std::cout<<"healthy\n";}
