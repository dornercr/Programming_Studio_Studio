#include <iostream>
#include <vector>

int main(){
 const int rows=1024, cols=1024; std::vector<int> m(rows*cols,1); long long sum=0;
 for(int r=0;r<rows;++r)
   for(int c=0;c<cols;++c)
     sum += m[r*cols+c];
 std::cout<<sum<<"\n";
}
