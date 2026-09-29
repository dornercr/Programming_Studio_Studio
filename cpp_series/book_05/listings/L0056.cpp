#include <memory>

int main(){
 auto data=std::make_unique<int[]>(4);
 data[4]=99; // intentional one-past-the-end write
}
