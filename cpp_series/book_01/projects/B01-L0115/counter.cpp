#include "counter.hpp"
int next_value(){ static int n=0; return ++n; }
