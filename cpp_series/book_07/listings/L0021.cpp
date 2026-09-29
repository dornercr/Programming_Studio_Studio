#include "harbor/check.hpp"
#include "harbor/domain.hpp"
#include <iostream>
#include <map>
struct Repository {
    virtual ~Repository()=default;
    virtual void put(harbor::Request)=0;
    virtual std::size_t size() const=0;
};
class MemoryRepository final : public Repository {
    std::map<std::string,harbor::Request> requests_;
public:
    void put(harbor::Request r) override { requests_.insert_or_assign(r.key,r); }
    std::size_t size() const override { return requests_.size(); }
};
void submit(Repository& repo, const harbor::Request& request) {
    (void)harbor::evaluate(request); repo.put(request);
}
int main() {
    MemoryRepository repository;
    submit(repository,{"a",3});
    harbor::check(repository.size()==1,"application policy");
    std::cout<<"Boundary is an interface, not necessarily a network hop.\n";
}
