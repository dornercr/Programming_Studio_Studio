#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <poll.h>
#include <iostream>

int main() {
    int fd[2];if(socketpair(AF_UNIX,SOCK_STREAM,0,fd)!=0)return 1;
    int flags=fcntl(fd[1],F_GETFL);if(flags<0||fcntl(fd[1],F_SETFL,flags|O_NONBLOCK)<0)return 2;
    char c=0;bool empty=recv(fd[1],&c,1,0)==-1&&(errno==EAGAIN||errno==EWOULDBLOCK);
    if(send(fd[0],"x",1,0)!=1)return 3;
    pollfd p{fd[1],POLLIN,0};if(poll(&p,1,1000)!=1||recv(fd[1],&c,1,0)!=1)return 4;
    bool drained=recv(fd[1],&c,1,0)==-1&&(errno==EAGAIN||errno==EWOULDBLOCK);
    close(fd[0]);close(fd[1]);std::cout<<std::boolalpha<<"empty="<<empty<<" byte="<<c<<" drained="<<drained<<'\n';
}
