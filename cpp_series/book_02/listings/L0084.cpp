#include <filesystem>
#include <iostream>
int main(){ namespace fs=std::filesystem; fs::path root="data"; fs::path file=root/"reports"/"run.txt"; std::cout<<file.generic_string()<<' '<<file.extension().string()<<'\n'; }
