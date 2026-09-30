int main(){Node root;root.left=std::make_unique<Node>();root.left->right=std::make_unique<Node>();std::cout<<height(&root)<<"\n";}
