#include <cassert>
int shipping_cost(int items){
    if(items <= 0) return 0;
    return items <= 3 ? 5 : 8;
}
int main(){
    assert(shipping_cost(0)==0);
    assert(shipping_cost(3)==5);
    assert(shipping_cost(4)==8);
}
