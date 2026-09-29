#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include <cstdio>
#include <memory>
#include <iostream>
int main(){std::unique_ptr<FILE,decltype(&fclose)> f(fopen("cert.pem","rb"),fclose);if(!f)return 1;
 std::unique_ptr<X509,decltype(&X509_free)> cert(PEM_read_X509(f.get(),nullptr,nullptr,nullptr),X509_free);if(!cert)return 2;
 int good=X509_check_host(cert.get(),"worker.test",0,0,nullptr);
 int bad=X509_check_host(cert.get(),"wrong.test",0,0,nullptr);
 if(good!=1||bad!=0)return 3;
 std::cout<<"worker.test accepted; wrong.test rejected\n";
}
