#include "harbor/check.hpp"
#include "harbor/queue.hpp"
#include "harbor/policies.hpp"
#include <iostream>
int main() {
    harbor::BoundedQueue<int> queue(2);
    harbor::check(queue.try_push(1) && queue.try_push(2),"admission");
    harbor::check(!queue.try_push(3),"capacity bound");
    queue.close();
    harbor::check(queue.pop()==1 && queue.pop()==2 && !queue.pop(),"close then drain");
    harbor::TokenBucket quota(2,10);
    harbor::check(quota.admit(2,0) && !quota.admit(1,0) && quota.admit(1,100),"quota refill");
    std::cout<<"Queue capacity bounds residence; token rate bounds admission frequency.\n";
}
