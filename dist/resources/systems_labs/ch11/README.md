# Check a copy before touching the destination

Copy a source string into an existing character array only if the characters and trailing null fit. Return false without writing anything when the capacity is too small.

## Acceptance checks

- cat needs capacity four including its trailing null.
- Capacity three rejects cat.
- Rejection preserves every destination byte.
- An empty string still needs one byte for the null.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch11/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The guard avoids computing source.size()+1, which could overflow at the type boundary. The caller must supply a live writable range of capacity bytes when capacity is nonzero. A check on a number cannot prove an arbitrary pointer is valid. An owned string is simpler when the external interface does not require a caller-owned buffer.

## Bug to diagnose

Accept source.size() <= capacity. An exactly full source then writes its null terminator one byte past the destination.

Require source.size() < capacity and test the exact-fit boundary including the terminator.
