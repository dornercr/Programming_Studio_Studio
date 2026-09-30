#include <iostream>
#include <vector>
int main(){const int rows=512,cols=512;std::vector<int>a(rows*cols,1);long row=0,col=0;for(int r=0;r<rows;++r)for(int c=0;c<cols;++c)row+=a[r*cols+c];for(int c=0;c<cols;++c)for(int r=0;r<rows;++r)col+=a[r*cols+c];std::cout<<row<<' '<<col<<'\n';}
