#include <cassert>
#include <vector>
double total(const std::vector<double>& values){ double sum=0; for(double v:values) sum+=v; return sum; }
int main(){
    assert(total({})==0.0);
    assert(total({2.5})==2.5);
    assert(total({1.0,2.0,3.0})==6.0);
}
