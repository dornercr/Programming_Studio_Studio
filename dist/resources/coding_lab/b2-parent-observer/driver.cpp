int main(){auto parent=std::make_shared<Node>();auto child=makeChild(parent);std::cout<<parent->children.size()<<" "<<static_cast<bool>(child->parent.lock())<<"\n";}
