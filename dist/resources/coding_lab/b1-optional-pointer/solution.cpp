#include <iostream>

int selectedValue(const int* selected,int fallback){ return selected?*selected:fallback; }
