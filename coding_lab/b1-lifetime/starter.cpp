#include <iostream>

class LiveToken{public:inline static int live=0;LiveToken(){++live;}~LiveToken(){}LiveToken(const LiveToken&)=delete;LiveToken& operator=(const LiveToken&)=delete;};
