#include <array>
#include <iostream>
#include <memory>
#include <vector>
using Page = std::array<int,2>;
using Pages = std::vector<std::shared_ptr<Page>>;
void write(Pages& pages, unsigned page, int value, unsigned& copies) {
    auto& owner = pages.at(page);
    if (!owner.unique()) { owner = std::make_shared<Page>(*owner); ++copies; }
    owner->at(0) = value;
}
int main() {
    Pages parent{std::make_shared<Page>(Page{7,8}),std::make_shared<Page>(Page{20,21})};
    Pages child = parent;
    unsigned copies = 0;
    write(child,0,9,copies);
    write(child,0,11,copies);
    std::cout << "parent=" << parent[0]->at(0) << " child=" << child[0]->at(0)
              << " copies=" << copies << '\n';
    std::cout << "untouched-shared=" << std::boolalpha << (parent[1] == child[1]) << '\n';
}
