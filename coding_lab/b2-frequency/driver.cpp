int main(){std::vector<std::string> words;for(std::string w;std::cin>>w;)words.push_back(w);for(const auto& [word,count]:frequencies(words))std::cout<<word<<":"<<count<<" ";std::cout<<"\n";}
