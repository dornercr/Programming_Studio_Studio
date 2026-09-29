#include <memory>
#include <stdexcept>

void unsafe(){
 int* p=new int[1024];
 throw std::runtime_error("failure");
 delete[] p; // unreachable
}

void safe(){
 auto p=std::make_unique<int[]>(1024);
 throw std::runtime_error("failure");
}
