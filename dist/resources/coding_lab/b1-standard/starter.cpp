#include <iostream>

bool supportsCxx20(long tag){ return tag>202002; // BUG: misses the exact C++20 tag
}
