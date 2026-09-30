#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>

int main(){
 int server=::socket(AF_INET,SOCK_STREAM,0); sockaddr_in a{}; a.sin_family=AF_INET; a.sin_addr.s_addr=htonl(INADDR_LOOPBACK); a.sin_port=0;
 if(::bind(server,reinterpret_cast<sockaddr*>(&a),sizeof a)!=0||::listen(server,1)!=0) return 1;
 socklen_t len=sizeof a; ::getsockname(server,reinterpret_cast<sockaddr*>(&a),&len);
 std::thread client([&]{int c=::socket(AF_INET,SOCK_STREAM,0);::connect(c,reinterpret_cast<sockaddr*>(&a),sizeof a);const char msg[]="ping";::send(c,msg,4,0);::close(c);});
 int peer=::accept(server,nullptr,nullptr); char buf[4]{}; ssize_t n=::recv(peer,buf,sizeof buf,MSG_WAITALL);
 std::cout.write(buf,n); std::cout<<"\n"; ::close(peer);::close(server);client.join();
}
