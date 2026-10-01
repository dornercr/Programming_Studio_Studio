#include <array>
#include <iostream>
using Graph = std::array<std::array<bool,3>,3>;
bool cycle(Graph reach) {
    for (int k=0; k<3; ++k)
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j)
                reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
    for (int i=0; i<3; ++i) if (reach[i][i]) return true;
    return false;
}
int main() {
    Graph ring{}, chain{};
    ring[0][1]=true; ring[1][2]=true; ring[2][0]=true;
    chain[0][1]=true; chain[1][2]=true;
    std::cout << std::boolalpha << "cycle=" << cycle(ring)
              << " chain=" << cycle(chain) << '\n';
}
