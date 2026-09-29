#pragma once
#include <cuda_runtime.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
inline void check(cudaError_t e,const char* operation) {
    if(e!=cudaSuccess) throw std::runtime_error(
        std::string(operation)+": "+cudaGetErrorString(e));
}
#define CU(call) check((call),#call)
template<class T> struct Device {
    T* p=nullptr;
    explicit Device(std::size_t n) {
        if(n) CU(cudaMalloc(reinterpret_cast<void**>(&p),n*sizeof(T)));
    }
    ~Device(){if(p) (void)cudaFree(p);}
    Device(const Device&)=delete;
    Device& operator=(const Device&)=delete;
};
struct Stream {
    cudaStream_t s{};
    Stream(){CU(cudaStreamCreateWithFlags(&s,cudaStreamNonBlocking));}
    ~Stream(){(void)cudaStreamSynchronize(s);(void)cudaStreamDestroy(s);}
    Stream(const Stream&)=delete;
    Stream& operator=(const Stream&)=delete;
};
struct Event {
    cudaEvent_t e{};
    Event(){CU(cudaEventCreate(&e));}
    ~Event(){(void)cudaEventDestroy(e);}
    Event(const Event&)=delete;
    Event& operator=(const Event&)=delete;
};
template<class T> struct Pinned {
    T* p=nullptr;
    explicit Pinned(std::size_t n){CU(cudaMallocHost(reinterpret_cast<void**>(&p),n*sizeof(T)));}
    ~Pinned(){(void)cudaFreeHost(p);}
    Pinned(const Pinned&)=delete;
    Pinned& operator=(const Pinned&)=delete;
};
inline void require(bool value,const char* message) {
    if(!value) throw std::runtime_error(message);
}
