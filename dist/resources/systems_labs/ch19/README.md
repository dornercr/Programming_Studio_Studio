# Validate a privileged-style range request

Model a protected service that accepts a length only when a numeric offset and extent fit inside a 16-byte allowed region. Reject both oversized lengths and invalid starting offsets.

## Acceptance checks

- Offset 4,length 8 is allowed.
- Offset 16,length 0 is allowed as an empty end range.
- Offset 17 is rejected; offset 15,length 2 is rejected.
- A maximum-size request cannot bypass the check by wrapping.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch19/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The trusted boundary checks the whole requested range, not only its length. The offset check comes first so extent-offset cannot underflow. This numeric model still cannot verify a real caller pointer or prevent a concurrent change to a mapping; an actual kernel copy operation must use the OS’s access rules and failure handling.

## Bug to diagnose

Check offset+length <= extent without checking overflow. A very large length can wrap the sum.

Check offset <= extent, then compare length with extent-offset. State what an empty range at the end means.
