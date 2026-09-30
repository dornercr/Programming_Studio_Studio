#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <csignal>
#include <charconv>
#include <cerrno>
#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>
struct FD { int value; explicit FD(int x):value(x){if(x<0)throw std::runtime_error("socket failure");}
 ~FD(){if(value>=0)::close(value);} FD(const FD&)=delete;FD&operator=(const FD&)=delete;};
volatile std::sig_atomic_t stopping=0;
extern "C" void stop_handler(int){stopping=1;}
int main(int argc,char**argv){try{
 std::signal(SIGTERM,stop_handler);std::signal(SIGINT,stop_handler);
 const bool container=argc==2&&std::string_view(argv[1])=="--container";
 FD listener(::socket(AF_INET,SOCK_STREAM,0));
 sockaddr_in address{};address.sin_family=AF_INET;
 address.sin_addr.s_addr=htonl(container?INADDR_ANY:INADDR_LOOPBACK);
 address.sin_port=htons(container?8080:0);
 if(::bind(listener.value,reinterpret_cast<sockaddr*>(&address),sizeof address)<0||::listen(listener.value,8)<0)return 2;
 socklen_t length=sizeof address;
 if(::getsockname(listener.value,reinterpret_cast<sockaddr*>(&address),&length)<0)return 3;
 std::cout<<"READY "<<ntohs(address.sin_port)<<'\n'<<std::flush;
 while(!stopping){
  pollfd ready{listener.value,POLLIN,0};int n=::poll(&ready,1,100);
  if(n<0){if(errno==EINTR)continue;return 4;}if(n==0)continue;
  int raw=::accept(listener.value,nullptr,nullptr);if(raw<0){if(errno==EINTR)continue;return 5;}FD client(raw);
  timeval timeout{1,0};::setsockopt(client.value,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof timeout);
  ::setsockopt(client.value,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof timeout);
  std::string request;char bytes[512];
  while(request.find("\r\n\r\n")==std::string::npos&&request.size()<4096){
   auto got=::recv(client.value,bytes,sizeof bytes,0);if(got<=0)break;
   request.append(bytes,static_cast<std::size_t>(got));
  }
  int status=400;std::string body="bad request\n";
  auto end=request.find("\r\n");
  if(request.size()<=4096&&end!=std::string::npos&&request.find("\r\n\r\n")!=std::string::npos){
   std::string_view line(request.data(),end);
   if(line.starts_with("GET ")&&line.ends_with(" HTTP/1.1")){
    auto path=line.substr(4,line.size()-4-9);
    if(path=="/healthz"||path=="/readyz"){status=200;body="ok\n";}
    else if(path.starts_with("/square?x=")){
     auto text=path.substr(10);int x{};auto [p,ec]=std::from_chars(text.data(),text.data()+text.size(),x);
     if(ec==std::errc{}&&p==text.data()+text.size()&&x>=0&&x<=100){status=200;body=std::to_string(x*x)+"\n";}
    }else{status=404;body="not found\n";}
   }
  }
  std::string response="HTTP/1.1 "+std::to_string(status)+(status==200?" OK":status==404?" Not Found":" Bad Request")+
   "\r\nContent-Type: text/plain\r\nConnection: close\r\nContent-Length: "+std::to_string(body.size())+"\r\n\r\n"+body;
  std::size_t sent=0;while(sent<response.size()){
   auto count=::send(client.value,response.data()+sent,response.size()-sent,MSG_NOSIGNAL);
   if(count<0&&errno==EINTR)continue;if(count<=0)break;sent+=static_cast<std::size_t>(count);
  }
 }
 std::cout<<"STOPPED\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 9;}}
