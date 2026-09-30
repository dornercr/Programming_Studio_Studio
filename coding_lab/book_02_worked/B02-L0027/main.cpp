#include <iostream>
#include <memory>
struct Parent;
struct Child{ std::weak_ptr<Parent> parent; };
struct Parent{ std::shared_ptr<Child> child=std::make_shared<Child>(); ~Parent(){std::cout<<"parent destroyed\n";} };
int main(){ auto p=std::make_shared<Parent>(); p->child->parent=p; }
