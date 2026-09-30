#include <string>
#include <iostream>

int main() {
    struct Endpoint{std::string host;unsigned port;};
    Endpoint bind{"0.0.0.0",8080},service{"harbor.default.svc.cluster.local",80};
    auto connectable=[](const Endpoint&e){return !e.host.empty()&&e.host!="0.0.0.0"&&e.port>0&&e.port<=65535;};
    std::cout<<"bind-client-address="<<std::boolalpha<<connectable(bind)<<" advertised="<<service.host<<':'<<service.port<<'\n';
}
