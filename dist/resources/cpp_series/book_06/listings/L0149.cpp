#include <fftw3.h>
#include <cmath>
#include <iostream>
int main(){
    auto* x=fftw_alloc_complex(4);auto* y=fftw_alloc_complex(4);
    if(!x||!y){fftw_free(x);fftw_free(y);return 1;}
    for(int i=0;i<4;++i){x[i][0]=(i==0);x[i][1]=0;}
    auto plan=fftw_plan_dft_1d(4,x,y,FFTW_FORWARD,FFTW_ESTIMATE);
    if(!plan){fftw_free(x);fftw_free(y);return 2;}
    fftw_execute(plan);bool valid=true;
    for(int i=0;i<4;++i)valid &= std::abs(y[i][0]-1)<1e-12&&std::abs(y[i][1])<1e-12;
    fftw_destroy_plan(plan);fftw_free(x);fftw_free(y);
    if(!valid)return 3;std::cout<<"FFTW impulse bins=1+0i\n";
}
