#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int main() {
    auto* value = static_cast<int*>(::mmap(nullptr,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0));
    if (value==MAP_FAILED) return 1; *value=10;
    pid_t pid=::fork();
    if(pid==0){ *value=25; _exit(0); }
    ::waitpid(pid,nullptr,0);
    std::cout << *value << "\n";
    ::munmap(value,sizeof(int));
}
