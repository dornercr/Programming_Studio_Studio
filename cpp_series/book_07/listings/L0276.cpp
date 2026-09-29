#include <iostream>
#include <algorithm>

int main() {
    auto rollout=[](int replicas,int per_replica,int required,int old_max,int new_min){
        bool capacity=(replicas-1)*per_replica>=required;bool compatible=new_min<=old_max;
        return replicas>1&&capacity&&compatible;};
    std::cout<<std::boolalpha<<"safe="<<rollout(3,100,180,2,1)<<" capacity-fail="<<rollout(2,100,180,2,1)
             <<" compatibility-fail="<<rollout(3,100,180,1,2)<<'\n';
}
