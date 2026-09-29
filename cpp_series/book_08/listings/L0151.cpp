#include <openssl/ssl.h>
#include <memory>
#include <iostream>

int main() {
    std::unique_ptr<SSL_CTX,decltype(&SSL_CTX_free)> ctx(SSL_CTX_new(TLS_client_method()),SSL_CTX_free);
    if(!ctx)return 1;SSL_CTX_set_verify(ctx.get(),SSL_VERIFY_PEER,nullptr);
    if(SSL_CTX_set_min_proto_version(ctx.get(),TLS1_2_VERSION)!=1)return 2;
    std::unique_ptr<SSL,decltype(&SSL_free)> ssl(SSL_new(ctx.get()),SSL_free);if(!ssl)return 3;
    if(SSL_set1_host(ssl.get(),"worker.test")!=1||SSL_set_tlsext_host_name(ssl.get(),"worker.test")!=1)return 4;
    if(SSL_get_verify_mode(ssl.get())!=SSL_VERIFY_PEER)return 5;
    std::cout<<"peer verification and worker.test identity configured; no handshake performed\n";
}
