#include <iostream>

int selectedValue(const int* selected,int fallback){ return selected?*selected:0; // BUG: ignores fallback
}
