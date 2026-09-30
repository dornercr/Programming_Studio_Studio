# Represent private and shared mappings

Use two maps whose entries own backing integers. Private mappings must point to distinct objects; one intentionally shared mapping must point to the same object in both maps.

## Acceptance checks

- Writes to A’s private address leave B’s private address unchanged.
- Writes to the shared address are visible through both maps.
- The two shared handles identify the same backing object.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch22/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The maps model address spaces and the owned integers model backing storage. Equal address keys do not imply equal objects. Sharing is represented by two entries holding handles to one backing object. The smart pointers operate within this demonstration process; they are not a cross-process shared-memory mechanism.

## Bug to diagnose

Create a fresh integer for each supposed shared mapping. The initial values match, but later writes do not propagate.

Share the backing object when the contract calls for sharing. Do not infer sharing from equal values or equal numeric virtual addresses.
