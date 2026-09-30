const int full = items / capacity;
const int remainder = items % capacity;
const int needed = full + (remainder != 0 ? 1 : 0);
