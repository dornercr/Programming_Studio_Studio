g++ -std=c++20 fftw_example.cpp -lfftw3 -o fftw_example
./fftw_example
nvcc -std=c++17 cufft_example.cu -lcufft -o cufft_example
./cufft_example
