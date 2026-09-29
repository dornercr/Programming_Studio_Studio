#include <type_traits>
template<class T> T twice(T v){ static_assert(std::is_arithmetic_v<T>,"twice requires arithmetic"); return v+v; }
int main(){ return twice(3)==6 ? 0 : 1; }
