#include "cuda_support.hpp"
struct Complex{float re,im;};
__device__ Complex multiply(Complex a,Complex b){return{a.re*b.re-a.im*b.im,a.re*b.im+a.im*b.re};}
__global__ void transform(const Complex* matrix,const Complex* x,Complex* y){
    unsigned row=threadIdx.x;if(row>=2)return;Complex sum{0,0};
    for(unsigned k=0;k<2;++k){auto v=multiply(matrix[row*2+k],x[k]);sum.re+=v.re;sum.im+=v.im;}y[row]=sum;
}

int main() {
    try {
        Complex matrix[4]{{1,0},{1,0},{1,0},{-1,0}},input[2]{{2,1},{1,-1}},output[2]{};
        Device<Complex>a(4),x(2),y(2);
        CU(cudaMemcpy(a.p,matrix,sizeof matrix,cudaMemcpyHostToDevice));CU(cudaMemcpy(x.p,input,sizeof input,cudaMemcpyHostToDevice));
        transform<<<1,32>>>(a.p,x.p,y.p);CU(cudaGetLastError());CU(cudaMemcpy(output,y.p,sizeof output,cudaMemcpyDeviceToHost));
        require(output[0].re==3&&output[0].im==0&&output[1].re==1&&output[1].im==2,"complex transform mismatch");
        std::cout<<"sum=3+0i difference=1+2i\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n'; return 1;
    }
}
