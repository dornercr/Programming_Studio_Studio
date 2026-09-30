#include <cstdio>
#include <stdexcept>
class File{ std::FILE* f_{}; public: explicit File(const char* p){ f_=std::fopen(p,"w"); if(!f_) throw std::runtime_error("open"); } ~File(){ if(f_) std::fclose(f_); } File(const File&)=delete; File& operator=(const File&)=delete; std::FILE* get() const{return f_;} };
int main(){ File f{"raii.txt"}; std::fputs("ok\n",f.get()); }
